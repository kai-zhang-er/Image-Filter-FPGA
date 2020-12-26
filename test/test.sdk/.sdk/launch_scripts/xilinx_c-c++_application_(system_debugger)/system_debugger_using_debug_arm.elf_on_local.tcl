connect -url tcp:127.0.0.1:3121
source /media/ck/B6DAFDC2DAFD7F45/program/ViROB307/after_check/test/test.sdk/design_1_wrapper_hw_platform_1/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zed 210248AC8CFA"} -index 0
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Zed 210248AC8CFA" && level==0} -index 1
fpga -file /media/ck/B6DAFDC2DAFD7F45/program/ViROB307/after_check/test/test.sdk/design_1_wrapper_hw_platform_1/design_1_wrapper.bit
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zed 210248AC8CFA"} -index 0
loadhw -hw /media/ck/B6DAFDC2DAFD7F45/program/ViROB307/after_check/test/test.sdk/design_1_wrapper_hw_platform_1/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zed 210248AC8CFA"} -index 0
ps7_init
ps7_post_config
configparams mdm-detect-bscan-mask 2
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zed 210248AC8CFA"} -index 0
dow /media/ck/B6DAFDC2DAFD7F45/program/ViROB307/after_check/test/test.sdk/arm/Debug/arm.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zed 210248AC8CFA"} -index 0
con
