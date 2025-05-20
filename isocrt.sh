head -n 1 isocrt.sh
#!/bin/bash
# Configuración de variables
IMAGE_DIR="./image"
ISO_OUTPUT="S-SUN.iso"
BOOT_FILE="bootx64.efi"
EFI_DIR="$IMAGE_DIR/efi/boot"
# Verificar si el archivo de arranque existe
if [ ! -f "$IMAGE_DIR/$BOOT_FILE" ]; then
    echo "ERROR: $BOOT_FILE no encontrado en $IMAGE_DIR."
    exit 1
fi
# Crear la estructura necesaria
mkdir -p "$EFI_DIR"
cp "$IMAGE_DIR/$BOOT_FILE" "$EFI_DIR"
# Crear la ISO usando mkisofs
mkisofs -o "$ISO_OUTPUT" -efi-boot "$EFI_DIR/$BOOT_FILE" -no-emul-boot -iso-level 3 -J -R "$IMAGE_DIR"
echo "ISO generada exitosamente: $ISO_OUTPUT"