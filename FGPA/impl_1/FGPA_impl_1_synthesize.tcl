if {[catch {

# define run engine funtion
source [file join {C:/lscc/radiant/3.2} scripts tcl flow run_engine.tcl]
# define global variables
global para
set para(gui_mode) 1
set para(prj_dir) "C:/Users/tdasari/balance-robot/FGPA"
# synthesize IPs
# synthesize VMs
# synthesize top design
file delete -force -- FGPA_impl_1.vm FGPA_impl_1.ldc
run_engine_newmsg synthesis -f "FGPA_impl_1_lattice.synproj"
run_postsyn [list -a iCE40UP -p iCE40UP5K -t SG48 -sp High-Performance_1.2V -oc Industrial -top -w -o FGPA_impl_1_syn.udb FGPA_impl_1.vm] "C:/Users/tdasari/balance-robot/FGPA/impl_1/FGPA_impl_1.ldc"

} out]} {
   runtime_log $out
   exit 1
}
