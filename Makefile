DIR = src
IMG = $(DIR)/imageprocessing.c
LIB = $(DIR)/trab2library.c
FILENAME_p = $(DIR)/main_p.c
FILENAME_s = $(DIR)/main_s.c
FILENAME_t = $(DIR)/main_t.c
TARGET_p = ./main_p
TARGET_s = ./main_s
TARGET_t = ./main_t
EXTENSIONS = *.c *.h *.sh *.png *.jpg *.md *.py *.o
USERNAME ?= $(USER)
ZIPFILE = $(USERNAME).zip
ZIP = zip

test: clean test_t test_p test_s
	python3 ./doc/plot_new.py
	make clean

main_t:
	gcc -o$(TARGET_t) $(FILENAME_t) $(IMG) $(LIB) -lm -lpthread -I./ -lfreeimage

test_t: clean main_t
	sh test.sh ./main_t > out_t.o
	cp out_t.o src/python/
	python3 ./$(DIR)/python/plot_t.py

main_p:
	gcc -o$(TARGET_p) $(FILENAME_p) $(IMG) $(LIB) -lm -I./ -lfreeimage

test_p: main_p
	sh test.sh ./main_p > out_p.o
	cp out_p.o src/python/
	python3 ./$(DIR)/python/plot_p.py

main_s:
	gcc -o$(TARGET_s) $(FILENAME_s) $(IMG) $(LIB) -lm -I./ -lfreeimage

test_s: main_s
	sh test.sh ./main_s > out_s.o
	cp out_s.o src/python/
	python3 ./$(DIR)/python/plot_s.py

clean:
	$(RM) ./$(TARGET_p)
	$(RM) ./$(TARGET_s)
	$(RM) ./$(TARGET_t)
	$(RM) ./*.o
	$(RM) ./doc/*.o

cleanALL:
	$(RM) ./$(TARGET_p)
	$(RM) ./*.jpg
	$(RM) ./$(TARGET_s)
	$(RM) ./$(TARGET_t)
	$(RM) ./*.o
	$(RM) ./*.png
	$(RM) ./doc/*.png
	$(RM) ./doc/*.o

debug_t: clean main_t
	./main_t 8 5
debug_p: clean main_p
	./main_p 8 1
debug_s: clean main_s
	./main_s 8 1

zip: clean
	$(ZIP) -R $(ZIPFILE)  Makefile $(EXTENSIONS)