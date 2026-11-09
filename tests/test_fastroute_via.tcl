read_lef "/home/eatheswar/silisfoundation/silis/third-party/pdks/sky130A/libs.ref/sky130_fd_sc_hd/techlef/sky130_fd_sc_hd__nom.tlef"
read_lef "/home/eatheswar/silisfoundation/silis/third-party/pdks/sky130A/libs.ref/sky130_fd_sc_hd/lef/sky130_fd_sc_hd.lef"
set tech [::ord::get_db_tech]
puts "=== DEFAULT TECH VIAS ==="
foreach via [$tech getVias] {
    puts "Via: [$via getName]"
}
quit
