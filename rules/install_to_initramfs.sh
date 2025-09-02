#!/usr/bin/env bash
# install_to_initramfs.sh — bundle real ELF executables from SRC into DEST (an initramfs root).
# - Copies real ELF executables into DEST/bin
# - Copies all shared libs (and the dynamic loader) flat into DEST/lib (as real files)
# - Sets RPATH to '$ORIGIN/../lib'
# - Sets the ELF interpreter to '/lib/<loader-name>' (absolute) to match initramfs layout
# - Ensures no symlinks in DEST/bin or DEST/lib
set -euo pipefail

die(){ echo "error: $*" >&2; exit 1; }
need(){ command -v "$1" >/dev/null 2>&1 || die "required tool '$1' not found"; }
need file; need ldd; need readelf; need patchelf; need realpath; need awk; need sed; need cp

SRC="${1:-}"; DEST="${2:-}"
[[ -n "${SRC}" && -n "${DEST}" ]] || die "usage: $0 SRC_DIR DEST_DIR"
[[ -d "$SRC" ]] || die "SRC_DIR '$SRC' is not a directory"

DEST="$(realpath -m "$DEST")"
BIN="$DEST/bin"; LIB="$DEST/lib"
mkdir -p "$BIN" "$LIB"

is_real_elf_exe(){ local p="$1"; [[ -f "$p" && ! -L "$p" ]] || return 1
  local info; info="$(file -b "$p" 2>/dev/null || true)"
  [[ "$info" == *ELF* && "$info" == *executable* && "$info" != *"shared object"* ]]
}
get_interp(){ readelf -l "$1" 2>/dev/null | awk '/Requesting program interpreter/ {print $NF}' | sed 's/]$//' ; }
list_ldd_paths(){ ldd "$1" 2>/dev/null | awk '
  /=>/ && $3 ~ /^\// {print $3}
  /^[[:space:]]*\// {print $1}
' | grep -vE '^linux-vdso\.so\.1$' | while read -r p; do realpath -e "$p" 2>/dev/null || true; done | awk 'NF' | sort -u; }
soname_or_base(){ local lib="$1"; local sn; sn="$(readelf -d "$lib" 2>/dev/null | awk '/SONAME/ {gsub(/\[|\]/,"",$5); print $5}')"
  [[ -n "${sn:-}" ]] && echo "$sn" || basename "$lib"; }
copy_real(){ cp --dereference --no-preserve=ownership --preserve=mode -f "$1" "$2"; chmod u+w "$2"; }

# collect executables
mapfile -t EXES < <(find "$SRC" -maxdepth 1 -type f ! -lname '*' -print | while read -r f; do is_real_elf_exe "$f" && echo "$f"; done)
[[ ${#EXES[@]} -gt 0 ]] || echo "No real ELF executables found in '$SRC'."

# bundle loaders (copied into DEST/lib, but will be referenced as /lib/<name> in ELF)
declare -A LOADER_NAME
for exe in "${EXES[@]}"; do
  interp="$(get_interp "$exe" || true)"
  if [[ -n "${interp:-}" && -e "$interp" ]]; then
    real_interp="$(realpath -e "$interp")"
    name="$(basename "$real_interp")"
    copy_real "$real_interp" "$LIB/$name"
    LOADER_NAME["$exe"]="$name"
  fi
done

# copy all deps (closure over libs-of-libs)
declare -A COPIED; declare -A NAMED
QUEUE=()
for exe in "${EXES[@]}"; do while read -r p; do [[ -n "$p" && -e "$p" ]] && QUEUE+=("$p"); done < <(list_ldd_paths "$exe"); done

for ((i=0;i<${#QUEUE[@]};i++)); do
  src="${QUEUE[$i]}"; key="$(realpath -e "$src" 2>/dev/null || echo "$src")"
  [[ -n "${COPIED["$key"]+x}" ]] && continue
  name="$(soname_or_base "$key")"
  if [[ -n "${NAMED["$name"]+x}" && ! -e "$LIB/$name" ]]; then
    suf="$(printf '%s' "$key" | awk '{print substr($0,length-7)}' | tr -cd '[:alnum:]')"
    name="$name.$suf"
  fi
  NAMED["$name"]=1
  copy_real "$key" "$LIB/$name"
  COPIED["$key"]=1
  while read -r sub; do [[ -n "$sub" && -e "$sub" ]] && QUEUE+=("$sub"); done < <(list_ldd_paths "$key")
done

# copy executables and patch
for exe in "${EXES[@]}"; do
  out="$BIN/$(basename "$exe")"
  copy_real "$exe" "$out"
  patchelf --remove-rpath "$out" 2>/dev/null || true
  patchelf --set-rpath '$ORIGIN/../lib' --force-rpath "$out"
  if [[ -n "${LOADER_NAME["$exe"]+x}" ]]; then
    # IMPORTANT: interpreter is absolute /lib/<name> for initramfs root
    patchelf --set-interpreter "/lib/${LOADER_NAME["$exe"]}" "$out"
  fi
done

# gap fix: ensure nothing still resolves outside DEST
fix=0
for exe in "${EXES[@]}"; do
  out="$BIN/$(basename "$exe")"
  while read -r p; do
    [[ -z "$p" ]] && continue
    case "$p" in
      "$LIB"/*) ;; # ok
      "$BIN"/*) ;; # unlikely
      /lib/*) ;;  # ok at runtime in initramfs (points to LIB inside DEST root)
      *)
        name="$(soname_or_base "$p")"
        copy_real "$p" "$LIB/$name"; fix=1
        ;;
    esac
  done < <(list_ldd_paths "$out")
done
if [[ $fix -eq 1 ]]; then
  for exe in "${EXES[@]}"; do
    out="$BIN/$(basename "$exe")"
    patchelf --remove-rpath "$out" 2>/dev/null || true
    patchelf --set-rpath '$ORIGIN/../lib' --force-rpath "$out"
  done
fi

# ensure no symlinks
if find "$BIN" "$LIB" -type l -print -quit | grep -q .; then
  die "found symlinks in '$BIN' or '$LIB' (expected only real files)"
fi