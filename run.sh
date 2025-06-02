#!/bin/bash
# qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker
# #qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512  -audiodev pa,id=speaker -machine pcspk-audiodev=speaker
if [[ "$1" = "gdb" ]]; then
  qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -d int
else
  qemu-system-x86_64  -hda Image/x64BareBonesImage.qcow2 -m 512 
fi

# env -i PATH=$PATH DISPLAY=$DISPLAY XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR ./run.sh  (de lolo)