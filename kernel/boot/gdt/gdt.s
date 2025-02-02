;;
;; bubbl
;; Copyright (C) 2024-2025  Raghuram Subramani <raghus2247@gmail.com>
;;
;; This program is free software: you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;;

global _GDT_flush
_GDT_flush:
  ;; First Argument (Pointer to the GDT)
  mov eax, [esp + 4]
  
  ;; Load GDT
  lgdt [eax]

  ;; Offset For Kernel Data Segment (16 bits)
  mov eax, 0x10

  ;; Set the Data Segment Selectors
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  ;; Set the Code Segment Selector
  jmp 0x08:.flush

.flush:
  ret
