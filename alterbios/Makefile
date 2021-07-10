OUTPUT=../ASYSGCC

all:
	make -C bios_patch
	make -C src	
	head -n -1 bios_patch/SYS_ALT.HEX > $(OUTPUT).HEX
	cat src/ALTBIOS.HEX >> $(OUTPUT).HEX
	echo 0x08004000 > $(OUTPUT)A.ADR
	arm-none-eabi-objcopy -I ihex $(OUTPUT).HEX -O binary $(OUTPUT)A.BIN

clean:
	make -C bios_patch clean
	make -C src clean
	rm -f $(OUTPUT).HEX $(OUTPUT)A.BIN $(OUTPUT)A.ADR

deploy: $(OUTPUT).HEX
	mount /mnt/dso
	cp $< /mnt/dso
	sync
	umount /mnt/dso

VERSION = $(shell git describe | tr -d '.-' | head -c 3)

publish:
	make clean all
	rm -rf api
	mkdir -p api
	make -C src api
	cp $(OUTPUT).HEX ALT_$(VERSION).HEX
	
	rm -f alterbios-api-$(VERSION).zip
	cd api; zip -r ../alterbios-api-$(VERSION).zip *
