add_to_path = $(eval PATH := $1:$(PATH))
current-dir = $(dir $(lastword $(MAKEFILE_LIST)))
all-makefiles-under = $(wildcard $(1)/*/module.mk)
all-subdir-makefiles = $(wildcard */module.mk)
