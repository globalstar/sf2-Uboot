#!/bin/bash
echo "Untar boot partition"
tar -xzvf boot-partition.tar.gz
MNTBOOT="/mnt/boot"
if [ ! -d  $MNTBOOT ]
then
  echo "Creating /mnt/boot directory"
  mkdir $MNTBOOT
fi

DEVICE="/dev/mmcblk0p1"
MMCBLK0P1=$(/sbin/fdisk -l | /bin/grep $DEVICE)
if [ "$MMCBLK0P1" ]
then
  echo "Mounting "$DEVICE" on "$MNTBOOT
  $(/bin/mount $DEVICE $MNTBOOT)

  if [ -f "MLO" ] && [ -f "u-boot.img" ]
  then
    echo "Preserving current U-Boot files"
    $(/bin/mv $MNTBOOT/MLO $MNTBOOT/old_MLO)
    $(/bin/mv $MNTBOOT/u-boot.img $MNTBOOT/old_u-boot.img)

    echo "Updating U-Boot files from /tmp directory"
    $(/bin/cp MLO $MNTBOOT/)
    $(/bin/cp u-boot.img $MNTBOOT/)
    echo "U-Boot files have been updated - Reboot system"
  else
    echo "ERROR: U-Boot files not in /tmp directory"
  fi

  echo "Unmounting "$DEVICE
  $(/bin/umount $DEVICE)
fi

