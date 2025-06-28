ls image

sudo chown -R root:root image/
sudo chmod -R 755 image/

xorriso -as mkisofs -o S-SUN.iso -iso-level 3 -volid "S-SUN" -J -R -append_partition 2 0xef image/efi/boot/BOOTX64.EFI -partition_cyl_align off -partition_offset 16 -boot_image any efi image/efi/boot/BOOTX64.EFI image/
  fdisk -l S-SUN.iso