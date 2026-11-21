read_lef "/home/eatheswar/silisfoundation/silis/third-party/pdks/sky130A/libs.ref/sky130_fd_sc_hd/techlef/sky130_fd_sc_hd__nom.tlef"
read_lef "/home/eatheswar/silisfoundation/silis/third-party/pdks/sky130A/libs.ref/sky130_fd_sc_hd/lef/sky130_fd_sc_hd.lef"
read_lef "/home/eatheswar/silisfoundation/silis/examples/cpp_macro_to_sv_chip/chip_top_proj/source/simple_alu.lef"
read_def "/home/eatheswar/silisfoundation/silis/examples/cpp_macro_to_sv_chip/chip_top_proj/results/temp.def"
catch { filler_placement "sky130_fd_sc_hd__decap_8 sky130_fd_sc_hd__decap_4 sky130_fd_sc_hd__fill_8 sky130_fd_sc_hd__fill_4 sky130_fd_sc_hd__fill_2 sky130_fd_sc_hd__fill_1" }
write_def "/home/eatheswar/silisfoundation/silis/examples/cpp_macro_to_sv_chip/chip_top_proj/results/chip_top_filled.def"
puts "SILIS_FILL_DONE"
