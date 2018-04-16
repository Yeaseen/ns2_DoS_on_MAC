static char code[] = "\n\
proc time2real v {\n\
global uscale\n\
\n\
foreach u [array names uscale] {\n\
set k [string first $u $v]\n\
if { $k >= 0 } {\n\
set scale $uscale($u)\n\
break\n\
}\n\
}\n\
if { $k > 0 } {\n\
set v [string range $v 0 [expr $k - 1]]\n\
set v [expr $scale * $v]\n\
}\n\
return $v\n\
}\n\
proc bw2real v {\n\
return [time2real $v]\n\
}\n\
\n\
proc step_format t {\n\
if { $t < 1e-3 } {\n\
return [format \"%.1f\" [expr $t * 1e6]]us\n\
} elseif { $t < 1. } {\n\
return [format \"%.1f\" [expr $t * 1e3]]ms\n\
}\n\
return [format \"%.1f\" $t]s\n\
}\n\
\n\
proc yesno attr {\n\
set v [resource $attr]\n\
if { [string match \\[0-9\\]* $v] } {\n\
return $v\n\
}\n\
if { $v == \"true\" || $v == \"True\" || $v == \"t\" || $v == \"y\" ||\n\
$v == \"yes\" } {\n\
return 1\n\
}\n\
return 0\n\
}\n\
\n\
proc resource s {\n\
return [option get . $s Nam]\n\
}\n\
\n\
proc mapf s { return $s }\n\
\n\
proc set_app_options { name } {\n\
global helv10 helv10b helv10o helv12 helv12b helv14 helv14b times14 \\\n\
ff tcl_platform\n\
\n\
option add $name.foundry adobe startupFile\n\
set ff [option get . foundry $name]\n\
\n\
set helv10 [mapf \"-$ff-helvetica-medium-r-normal--*-100-75-75-*-*-*-*\"]\n\
set helv10b [mapf \"-$ff-helvetica-bold-r-normal--*-100-75-75-*-*-*-*\"]\n\
set helv10o [mapf \"-$ff-helvetica-bold-o-normal--*-100-75-75-*-*-*-*\"]\n\
set helv12 [mapf \"-$ff-helvetica-medium-r-normal--*-120-75-75-*-*-*-*\"]\n\
set helv12b [mapf \"-$ff-helvetica-bold-r-normal--*-120-75-75-*-*-*-*\"]\n\
set helv14 [mapf \"-$ff-helvetica-medium-r-normal--*-140-75-75-*-*-*-*\"]\n\
set helv14b [mapf \"-$ff-helvetica-bold-r-normal--*-140-75-75-*-*-*-*\"]\n\
set times14 [mapf  \"-$ff-times-medium-r-normal--*-140-75-75-*-*-*-*\"]\n\
\n\
if {$tcl_platform(platform)!=\"windows\"} {\n\
option add *font $helv12b startupFile\n\
option add *Font $helv12b startupFile\n\
}\n\
\n\
option add *Balloon*background yellow widgetDefault\n\
option add *Balloon*foreground black widgetDefault\n\
option add *Balloon.info.wrapLegnth 3i widgetDefault\n\
option add *Balloon.info.justify left widgetDefault\n\
option add *Balloon.info.font \\\n\
[mapf \"-$ff-lucida-medium-r-normal-sans-*-120-*\"] widgetDefault\n\
\n\
option add $name.disablefont $helv10o startupFile\n\
option add $name.smallfont $helv10b startupFile\n\
option add $name.medfont $helv12b  startupFile\n\
option add $name.helpFont $times14 startupFile\n\
option add $name.entryFont $helv10 startupFile\n\
\n\
option add $name.rate		2ms	startupFile\n\
option add $name.movie		0	startupFile\n\
option add $name.granularity	40	startupFile\n\
option add $name.pause		1	startupFile\n\
}\n\
\n\
if {$tcl_platform(platform)!=\"windows\"} {\n\
option add *foreground black startupFile\n\
option add *background gray80 startupFile\n\
option add *activeForeground black startupFile\n\
option add *activeBackground gray90 startupFile\n\
option add *viewBackground gray90 startupFile\n\
option add *disabledForeground gray50 startupFile\n\
\n\
} else {\n\
option add *foreground SystemButtonText startupFile\n\
option add *background SystemButtonFace startupFile\n\
option add *viewBackground SystemButtonHighlight startupFile\n\
option add *activeForeground SystemHighlightText startupFile\n\
option add *activeBackground SystemHighlight startupFile\n\
option add *disabledForeground SystemDisabledText startupFile\n\
}\n\
\n\
option add *Radiobutton.relief flat startupFile\n\
\n\
option add *padX 2 startupFile\n\
option add *padY 2 startupFile\n\
option add *tearOff 0 startupFile\n\
\n\
proc smallfont { } {\n\
return [option get . smallfont Nam]\n\
}\n\
proc mediumfont { } {\n\
return [option get . medfont Nam]\n\
}\n\
\n\
proc nam_angle { v } {\n\
switch -regexp $v {\n\
^[0-9\\.]+deg$    { regexp  {([0-9\\.]+)deg$} $v a dval\n\
return [expr {fmod($dval,360)/180}] }\n\
\n\
^[0-9\\.]+$	{ return $v }\n\
\n\
^up-right$ -\n\
^right-up$	{ return 0.25 }\n\
\n\
\n\
^up$		{ return 0.5 }\n\
\n\
^up-left$ -\n\
^left-up$	{ return 0.75 }\n\
\n\
^left$		{ return 1. }\n\
\n\
^left-down$ -\n\
^down-left$	{ return 1.25 }\n\
\n\
^down$		{ return 1.5 }\n\
\n\
^down-right$ -\n\
^right-down$	{ return 1.75 }\n\
\n\
default		{ return 0.0 }\n\
}\n\
}\n\
\n\
proc remote_cmd { async interp cmd } {\n\
if $async {\n\
set rcmd \"send -async \\\"$interp\\\" {$cmd}\"\n\
} else {\n\
set rcmd \"send \\\"$interp\\\" {$cmd}\"\n\
}\n\
eval $rcmd\n\
}\n\
\n\
proc get_trace_item { tag traceevent} {\n\
set next 0\n\
foreach item $traceevent {\n\
if { $next == 1 } {\n\
return $item\n\
}\n\
if { $item == $tag } {\n\
set next 1\n\
} \n\
}\n\
return \"\"\n\
}\n\
\n\
proc place_frame { w f x y } {\n\
set nh [winfo height $w]\n\
set nw [winfo width $w]\n\
place $f -x $nw -y $nh -bordermode outside\n\
update\n\
set ih [winfo reqheight $f]\n\
set iw [winfo reqwidth $f]\n\
set px $x\n\
set py $y\n\
if {($px + $iw) > $nw} {\n\
set px [expr $x - $iw]\n\
if {$px < 0} { set px 0 }\n\
}\n\
if {($py + $ih) > $nh} {\n\
set py [expr $y - $ih]\n\
if {$py < 0} {set py 0}\n\
}\n\
place $f -x $px -y $py -bordermode outside\n\
}\n\
\n\
\n\
\n\
\n\
Class AnimControl \n\
\n\
AnimControl instproc init {trace_file args} {\n\
AnimControl instvar PORT_FILE_\n\
\n\
catch \"array set opts $args\"\n\
if [info exists opts(s)] {\n\
AnimControl instvar isSync_\n\
set isSync_ 1\n\
}\n\
if [info exists opts(a)] {\n\
set flag 1\n\
} elseif {[file exists $PORT_FILE_] && [file readable $PORT_FILE_]} {\n\
set flag 0\n\
} else {\n\
set flag 1\n\
}\n\
if [info exists opts(k)] {\n\
AnimControl instvar INIT_PORT_\n\
set INIT_PORT_ $opts(k)\n\
}\n\
if $flag {\n\
$self local-create-animator $trace_file [join $args]\n\
} else {\n\
$self remote-create-animator $trace_file [join $args]\n\
}\n\
}\n\
\n\
proc ::AnimCtrlOnRemoteRequest args {\n\
eval [AnimControl instance] on-remote-request $args\n\
}\n\
\n\
AnimControl instproc on-remote-request { newsock addr port } {\n\
\n\
gets $newsock line\n\
set buf [split $line]\n\
switch [lindex $buf 0] {\n\
CA {\n\
set trace_file [lindex $buf 1]\n\
regsub -all {\\\\} $trace_file {\\\\\\\\} trace_file\n\
set args [lrange $buf 2 end]\n\
eval $self create-animator $trace_file $args\n\
}\n\
default {\n\
puts \"Unsupported command [lindex $buf 0]\"\n\
}\n\
}\n\
close $newsock\n\
}\n\
\n\
AnimControl instproc start-server {} {\n\
$self instvar NAM_PORT_ NAM_SOCK_\n\
AnimControl instvar INIT_PORT_ PORT_FILE_\n\
set NAM_PORT_ $INIT_PORT_\n\
\n\
set ret 1\n\
while {$ret} {\n\
incr NAM_PORT_\n\
set ret [catch {set NAM_SOCK_ \\\n\
[socket -server ::AnimCtrlOnRemoteRequest $NAM_PORT_]}]\n\
if {$ret} {\n\
if [info exists NAM_SOCK_] {\n\
close $NAM_SOCK_\n\
}\n\
if {$NAM_PORT_ - $INIT_PORT_ > 254} {\n\
error \"Nam failed to create a socket ranging \\\n\
from $INIT_PORT_ to $NAM_PORT_.\"\n\
}\n\
}\n\
}\n\
\n\
set f [open $PORT_FILE_ w 0600]\n\
puts -nonewline $f $NAM_PORT_\n\
close $f\n\
}\n\
\n\
AnimControl instproc remote-create-animator {trace_file args} {\n\
catch \"array set opts $args\"\n\
AnimControl instvar PORT_FILE_\n\
if [catch {set f [open $PORT_FILE_ RDONLY]}] {\n\
error \"Cannot read server port from $PORT_FILE_\"\n\
}\n\
set port [read $f]\n\
close $f\n\
\n\
if [catch {set sock [socket localhost $port]}] {\n\
puts -nonewline \"Cannot connect to existing nam instance. \"\n\
puts \"Starting a new one...\"\n\
$self local-create-animator $trace_file [join $args]\n\
} else {\n\
if {$trace_file == \"\"} {\n\
puts \"A nam instance already exists. Use nam <trace file> to view an animation\"\n\
close $sock\n\
} else {\n\
global tcl_platform\n\
if {$tcl_platform(platform) == \"windows\"} {\n\
if [regexp {^(\\\\\\\\|[A-Za-z]:[/\\\\])} $trace_file] {\n\
set tf $trace_file	;# Absolute pathname\n\
} else {\n\
set tf [pwd]/$trace_file	;# Relative pathname\n\
}\n\
} else {\n\
if [regexp {^[~/]} $trace_file] {\n\
set tf $trace_file	;# Absolute pathname\n\
} else {\n\
set tf [pwd]/$trace_file ;# Relative pathname\n\
}\n\
}\n\
puts $sock \"CA $tf [join $args]\"\n\
flush $sock\n\
close $sock\n\
}\n\
}\n\
}\n\
\n\
AnimControl instproc local-init {} {\n\
$self instvar nam_name wix tagnum animators_\n\
\n\
. configure -background [option get . background Nam]\n\
set_app_options [tk appname]\n\
\n\
set nam_name \"NAM - The Network Animator v[version]\"\n\
set wix 0\n\
set tagnum 0\n\
set animators_ {}\n\
\n\
$self build-menu \n\
$self new_webhome \n\
$self bind-window .\n\
\n\
AnimControl instvar instance_ \n\
if {$instance_ == \"\"} { \n\
set instance_ $self \n\
} else {\n\
error \"Couldn't instantiate more than one AnimControl!\"\n\
}\n\
\n\
wm title . \"Nam Console v[version]\"\n\
\n\
global nam_local_display\n\
set nam_local_display 1\n\
}\n\
\n\
AnimControl proc instance {} {\n\
set ac [AnimControl set instance_]\n\
if {$ac != \"\"} {\n\
return $ac\n\
}\n\
error \"Couldn't find instance of AnimControl\"\n\
}\n\
\n\
AnimControl instproc bind-window { w } {\n\
bind $w <Control-o> \"$self on-open\"\n\
bind $w <Control-O> \"$self on-open\"\n\
bind $w <Control-q> \"$self done\"\n\
bind $w <Control-Q> \"$self done\"\n\
bind $w <Control-n> \"$self on-new\"\n\
bind $w <Control-N> \"$self on-new\"\n\
bind $w <Control-w> \"$self on-winlist\"\n\
bind $w <Control-W> \"$self on-winlist\"\n\
bind $w <q> \"$self done\"\n\
wm protocol $w WM_DELETE_WINDOW \"$self done\"\n\
}\n\
\n\
AnimControl instproc build-menu {} {\n\
$self instvar nam_name\n\
\n\
frame .menu -relief groove -bd 2\n\
pack .menu -side top -fill x\n\
set padx 4\n\
\n\
set mb .menu.file\n\
set m $mb.m\n\
menubutton $mb -text \"File\" -menu $m -underline 0 \\\n\
-borderwidth 1 \n\
menu $m\n\
$m add command -label \"New Nam Editor... Ctrl+N\" -command \"$self on-new\"\n\
$m add command -label \"Open... Ctrl+O\" -command \"$self on-open\"\n\
$m add command -label \"WinList Ctrl+W\" -command \"$self on-winlist\"\n\
$m add separator\n\
$m add command -label \"Quit    Ctrl+Q\" -command \"$self done\"\n\
pack $mb -side left -padx $padx\n\
\n\
label .menu.name -text $nam_name -font [smallfont] \\\n\
-width 30 -borderwidth 2 -relief groove \n\
pack .menu.name -side left -fill x -expand true -padx 4 -pady 1\n\
\n\
set mb .menu.help\n\
set m $mb.m\n\
menubutton $mb -text \"Help\" -menu $m -underline 0 \\\n\
-borderwidth 1 \n\
menu $m\n\
$m add command -label \"Help\" -command \"$self new_web help\"\n\
$m add command -label \"About nam\" -command \"$self new_web about\"\n\
\n\
\n\
pack $mb -side right -padx $padx\n\
}\n\
\n\
AnimControl instproc done {} {\n\
AnimControl instvar PORT_FILE_\n\
$self instvar animators_ NAM_SOCK_ NAM_PORT_\n\
\n\
if {[llength $animators_] > 0 && \\\n\
[tk_messageBox -message \"Really quit?\" -type yesno -icon question \\\n\
-title \"nam\" -default \"no\"] == \"no\"} {\n\
return\n\
}\n\
\n\
close $NAM_SOCK_\n\
if [catch \"set f [open $PORT_FILE_ r]\"] {\n\
puts \"Nam port file already deleted.\"\n\
} else {\n\
set buf [gets $f]\n\
close $f\n\
if {$buf == $NAM_PORT_} {\n\
file delete $PORT_FILE_\n\
} else {\n\
puts \"Another nam instance is running...\"\n\
}\n\
}\n\
foreach a $animators_ {\n\
$a done\n\
}\n\
destroy .\n\
}\n\
\n\
AnimControl instproc on-open {} {\n\
$self tkvar openFile_ openPeer_\n\
\n\
\n\
set openFile_ [tk_getOpenFile \\\n\
-filetypes {{{NAM} {.nam .nam.gz}} {{Nam Editor} {.ns .enam}}} \\\n\
-parent .]\n\
\n\
if {$openFile_ == \"\"} {return}\n\
\n\
set w .openFile\n\
set openPeer_ \"\"\n\
\n\
$self do-open-tracefile $w\n\
\n\
return\n\
\n\
set openPeer_ \"\"\n\
\n\
toplevel .openFile\n\
set w .openFile\n\
\n\
wm title $w \"Open Trace File\"\n\
frame $w.f -borderwidth 1 -relief raised\n\
pack $w.f -side top -fill both -expand true\n\
filesel $w.f.sel $w -variable [$self tkvarname openFile_] \\\n\
-command \"$self do-open-tracefile $w.f.sel\" \\\n\
-label \"Trace file to open:\"\n\
pack $w.f.sel -side top -fill both -expand true\n\
\n\
label $w.pl0 -text \"Peer:\"\n\
pack $w.pl0 -side top -anchor w\n\
entry $w.peerentry -width 50 -relief sunken \\\n\
-textvariable [$self tkvar openPeer_]\n\
pack $w.peerentry -side top -fill both -expand true\n\
\n\
frame $w.f.btns -borderwidth 0\n\
pack $w.f.btns -side top -fill x\n\
button $w.f.btns.save -text Open \\\n\
-command \"$self do-open-tracefile $w\"\n\
pack $w.f.btns.save -side left -fill x -expand true\n\
button $w.f.btns.cancel -text Cancel -command \"destroy $w\"\n\
pack $w.f.btns.cancel -side left -fill x -expand true\n\
set openFile_ [pwd]\n\
}\n\
\n\
AnimControl instproc do-open-tracefile {w} {\n\
$self tkvar openFile_ openPeer_\n\
checkfile $openFile_ \"r\" $w.f.sel $w \\\n\
\"$self create-animator $openFile_ p=$openPeer_\"\n\
}\n\
\n\
AnimControl instproc local-create-animator { trace_file args } {\n\
$self local-init\n\
$self start-server\n\
if {$trace_file != \"\"} {\n\
$self create-animator $trace_file [join $args]\n\
}\n\
}\n\
\n\
AnimControl instproc create-animator { tracefile args } {\n\
$self instvar animators_\n\
AnimControl instvar ANIMATOR_CLASS_\n\
\n\
set fnsets [split $tracefile \".\"]\n\
set fnlength [llength $fnsets]\n\
set ftype [lindex $fnsets [expr $fnlength-1]]\n\
set ftype [string tolower $ftype]\n\
\n\
if {[string compare $ftype \"ns\"] == 0  || \n\
[string compare $ftype \"enam\"] == 0 } {\n\
\n\
set ANIMATOR_CLASS_ Editor\n\
set editor [new $ANIMATOR_CLASS_ $tracefile]\n\
return\n\
}\n\
\n\
set ANIMATOR_CLASS_ Animator\n\
AnimControl instvar isSync_\n\
if {[info exist isSync_]} {\n\
lappend args {s 1}\n\
}\n\
if ![catch {eval set anim [new $ANIMATOR_CLASS_ $tracefile \\\n\
[join $args]]} errMsg] {\n\
lappend animators_ $anim\n\
$self instvar winlist_\n\
if [info exists winlist_] {\n\
$winlist_ insert end [$anim get-name]\n\
}\n\
} else {\n\
global errorInfo\n\
puts \"Failed to start animator: \"\n\
puts \"$errMsg\"\n\
puts \"$errorInfo\"\n\
}\n\
}\n\
\n\
AnimControl instproc on-new {} {\n\
$self instvar animators_\n\
AnimControl instvar ANIMATOR_CLASS_\n\
set ANIMATOR_CLASS_ Editor\n\
set editor [new $ANIMATOR_CLASS_ \"\"]\n\
}\n\
\n\
\n\
AnimControl instproc close-animator { animator } {\n\
$self instvar animators_\n\
set pos [lsearch $animators_ $animator]\n\
set animators_ [lreplace $animators_ $pos $pos]\n\
$self instvar winlist_\n\
if [info exists winlist_] {\n\
$winlist_ delete $pos\n\
}\n\
}\n\
\n\
AnimControl instproc on-winlist {} {\n\
$self instvar winlist_ \n\
if [info exists winlist_] {\n\
return\n\
}\n\
\n\
set w .winList\n\
toplevel $w\n\
\n\
wm title $w \"Active Animations\"\n\
\n\
frame $w.f -borderwidth 0 -highlightthickness 0\n\
frame $w.f.f\n\
listbox $w.f.f.a -xscrollcommand \"$w.f.f.ah set\" \\\n\
-yscrollcommand \"$w.f.f2.av set\" -height 10 \\\n\
-selectmode single \n\
pack $w.f.f.a -fill both -side top -expand true \n\
set winlist_ $w.f.f.a\n\
\n\
scrollbar $w.f.f.ah -orient horizontal -width 10 -borderwidth 1 \\\n\
-command \"$w.f.f.a xview\"\n\
$w.f.f.ah set 0.0 1.0\n\
pack $w.f.f.ah -side bottom -fill x -expand true\n\
\n\
frame $w.f.f2\n\
pack $w.f.f2 -side left -fill y\n\
scrollbar $w.f.f2.av -orient vertical -width 10 -borderwidth 1 \\\n\
-command \"$w.f.f.a yview\"\n\
$w.f.f2.av set 0.0 1.0\n\
pack $w.f.f2.av -side top -fill y -expand true\n\
pack $w.f.f -side left -fill both -expand true\n\
pack $w.f -side left -fill both -expand true\n\
\n\
bind $w.f.f.a <Double-ButtonPress-1> \"$self winlist_show $w.f.f.a\"\n\
bind $w.f.f.a <Destroy> \"+$self unset winlist_\"\n\
bind $w <Control-q> \"[AnimControl instance] done\"\n\
bind $w <Control-Q> \"[AnimControl instance] done\"\n\
bind $w <Control-w> \"destroy $w\"\n\
bind $w <Control-W> \"destroy $w\"\n\
bind $w <Escape> \"destroy $w\"\n\
\n\
$self instvar animators_\n\
foreach a $animators_ {\n\
$w.f.f.a insert end [$a get-name]\n\
}\n\
}\n\
\n\
AnimControl instproc winlist_show { wlbox } {\n\
set idx [$wlbox index active]\n\
$self instvar animators_\n\
if [llength $animators_] then {\n\
raise [[lindex $animators_ $idx] get-tkwin]\n\
}\n\
}\n\
\n\
Animator instproc init { trace_file args } {\n\
$self instvar windows trace tracefile \\\n\
prevRate rateSlider currRate netModel \\\n\
running peers peerName granularity direction \\\n\
nam_name graphs NETWORK_MODEL nam_record_filename \\\n\
nam_record_frame id_ sliderPressed maxmon analysis_flag \\\n\
colorname fcolorname now mintime maxtime prevTime range \\\n\
welcome analysis_OK analysis_ready nam_version viewctr \\\n\
plotmarks maxtimeflag pipemode enable_edit_ showenergy_ \\\n\
backward do_validate_nam\n\
\n\
$self instvar observerlist_\n\
set observerlist_ \"\"\n\
\n\
$self next\n\
\n\
set id_ [Animator set id_]\n\
Animator set id_ [incr id_]\n\
\n\
Animator instvar SyncAnimator_\n\
if ![info exists SyncAnimator_] {\n\
set SyncAnimator_ {}  ;# Empty list\n\
}\n\
$self tkvar isSync_\n\
set isSync_ 0\n\
\n\
set tracefile $trace_file\n\
set nam_name $tracefile\n\
\n\
set showenergy_ 0\n\
set analysis_flag 0\n\
set analysis_OK 0\n\
set analysis_ready 0\n\
set nam_version 0\n\
set welcome \"                              Welcome to NamGraph 1.15                               \"\n\
\n\
set NETWORK_MODEL NetworkModel\n\
\n\
set nam_record_filename nam\n\
set nam_record_frame 0\n\
\n\
set sliderPressed 0\n\
set direction 1\n\
set running 0\n\
\n\
set backward 0\n\
set do_validate_nam 0\n\
\n\
set viewctr 0\n\
set maxmon 0\n\
\n\
set colorname(0) black\n\
set colorname(1) navy\n\
set colorname(2) cornflowerblue\n\
set colorname(3) blue\n\
set colorname(4) deepskyblue\n\
set colorname(5) steelblue\n\
set colorname(6) dodgerblue\n\
set colorname(7) darkolivegreen\n\
\n\
set fcolorname(0) red\n\
set fcolorname(1) brown\n\
set fcolorname(2) purple\n\
set fcolorname(3) orange\n\
set fcolorname(4) chocolate\n\
set fcolorname(5) salmon\n\
set fcolorname(6) greenyellow\n\
set fcolorname(7) gold\n\
\n\
if ![info exists plotmarks] {\n\
set plotmarks(0) mark1\n\
set plotmarks(1) mark2\n\
set plotmarks(2) mark3\n\
set plotmarks(3) mark4\n\
set plotmarks(4) mark5\n\
set plotmarks(5) mark6\n\
set plotmarks(6) mark7\n\
set plotmarks(7) mark8\n\
set plotmarks(8) mark1\n\
set plotmarks(9) mark2\n\
set plotmarks(10) mark3\n\
set plotmarks(11) mark4\n\
set plotmarks(12) mark5\n\
set plotmarks(13) mark6\n\
set plotmarks(14) mark7 \n\
set plotmarks(15) mark8\n\
}\n\
\n\
set graphs \"\"\n\
set statsViews \"\"\n\
\n\
$self infer-network-model $tracefile\n\
$self nam_analysis $tracefile\n\
\n\
set netModel [new $NETWORK_MODEL $self $tracefile]\n\
\n\
if [catch {\n\
set trace [new Trace $tracefile $self]\n\
set now [$trace mintime]\n\
set mintime $now\n\
set maxtime [expr [$trace maxtime] + .05]\n\
set pipemode 0\n\
if { [$trace maxtime] == 0 } {\n\
set maxtime 3.0\n\
set pipemode 1\n\
set maxtimeflag 0 \n\
}\n\
set range [expr $maxtime - $mintime]\n\
set prevTime $mintime\n\
$trace connect $netModel\n\
} errMsg] {\n\
error \"$errMsg\" \n\
}\n\
\n\
$self build-ui\n\
\n\
Animator instvar INIT_RATE_ INIT_TIME_\n\
if [info exists INIT_RATE_] {\n\
set currRate [expr 10.0 * log10([time2real $INIT_RATE_])]\n\
Animator unset INIT_RATE_\n\
}\n\
if [info exists INIT_TIME_] {\n\
set now [time2real $INIT_TIME_]\n\
Animator unset INIT_TIME_\n\
}\n\
\n\
set peerName \"\"\n\
set peers \"\"\n\
if {$args != \"\"} {\n\
catch \"array set opts $args\"\n\
if [info exists opts(p)] {\n\
set peerName $opts(p)\n\
peer_init $peerName\n\
}\n\
if [info exists opts(f)] {\n\
if [file exists $opts(f)] {\n\
source $opts(f)\n\
}\n\
}\n\
if [info exists opts(j)] {\n\
set now [time2real $opts(j)]\n\
}\n\
if [info exists opts(r)] {\n\
set currRate [expr 10.0 * log10([time2real $opts(r)])]\n\
}\n\
if [info exists opts(z)] {\n\
set do_validate_nam 1\n\
}    \n\
if [info exists opts(s)] {\n\
set isSync_ 1\n\
}\n\
}\n\
\n\
if [catch {\n\
$self settime $now\n\
$self set_rate $currRate 1\n\
} errMsg] {\n\
error \"$errMsg\" \n\
}\n\
\n\
if { $showenergy_ == 1 } {\n\
$self energy_view\n\
}\n\
\n\
$trace connect [new TraceHook $self]\n\
\n\
if { $do_validate_nam == 1 } {\n\
$self play 1\n\
}\n\
}\n\
\n\
Animator instproc infer-network-model { tracefile } {\n\
$self instvar NETWORK_MODEL\n\
\n\
set stream [new NamStream $tracefile]\n\
set NETWORK_MODEL NetworkModel\n\
\n\
if { ![regexp \"^_\" $stream] } {\n\
puts \"\\nnam: Unable to open the file \\\"$tracefile\\\"\\n\"\n\
exit\n\
}\n\
\n\
set time \"0\"\n\
while {([$stream eof]==0)&&([string compare $time \"*\"]!=0)} {\n\
set line [$stream gets]\n\
set time [get_trace_item \"-t\" $line]\n\
}\n\
\n\
set num_link 0 \n\
while {([$stream eof]==0)&&([string compare $time \"*\"]==0)} {\n\
set cmd [lindex $line 0]\n\
set time [get_trace_item \"-t\" $line]\n\
if {[string compare $time \"*\"]!=0} {\n\
break\n\
}\n\
\n\
switch $cmd {\n\
\"l\" {\n\
set direction [get_trace_item \"-O\" $line]\n\
if {$direction==\"\"} {\n\
set direction [get_trace_item \"-o\" $line]\n\
}\n\
if {$direction==\"\"} {\n\
set NETWORK_MODEL NetworkModel/Auto\n\
}\n\
incr num_link\n\
}\n\
\"W\" {\n\
set NETWORK_MODEL NetworkModel/Wireless\n\
set width [get_trace_item \"-x\" $line]\n\
set height [get_trace_item \"-y\" $line]\n\
NetworkModel/Wireless set Wpxmax_ $width\n\
NetworkModel/Wireless set Wpymax_ $height\n\
break\n\
}\n\
}\n\
set line [$stream gets]\n\
}\n\
$stream close\n\
if {$num_link == 0 && [string compare $NETWORK_MODEL \"NetworkModel/Wireless\"] !=0 } {\n\
set NETWORK_MODEL NetworkModel/Auto\n\
}\n\
}\n\
\n\
Animator instproc peer_init { name handle } {\n\
$self peer $name 0 $handle\n\
$self peer_cmd 0 \"peer \\\"[tk appname]\\\" 1 $self\"\n\
}\n\
\n\
Animator instproc peer_cmd { async cmd } {\n\
$self instvar peers\n\
foreach s $peers {\n\
remote_cmd $async [lindex $s 0] \"[lindex $s 1] $cmd\"\n\
}\n\
$self sync_cmd $cmd\n\
}\n\
\n\
Animator instproc sync_cmd { cmd } {\n\
$self tkvar isSync_\n\
if !$isSync_ {\n\
return\n\
}\n\
Animator instvar SyncAnimator_\n\
foreach s $SyncAnimator_ {\n\
if {$s == $self} { continue }\n\
eval $s $cmd\n\
}\n\
}\n\
\n\
Animator instproc peer { name remote handle } {\n\
$self instvar peers\n\
if { $remote } {\n\
peer_cmd 1 \"peer \\\"$name\\\" 0 $self\"\n\
foreach s $peers {\n\
set p [lindex $s 0]\n\
set h [lindex $s 1]\n\
remote_cmd 1 $name \"peer \\\"$p\\\" 0 $h\"\n\
}\n\
}\n\
lappend peers [list $name $handle]\n\
}\n\
\n\
Animator instproc backFrame { } {\n\
$self instvar now timeStep\n\
$self settime [expr $now - $timeStep]\n\
}\n\
\n\
Animator instproc nextFrame { } {\n\
$self instvar now timeStep direction\n\
if {$direction > 0} {\n\
$self settime [expr $now + $timeStep]\n\
} else {\n\
$self settime [expr $now - $timeStep]\n\
}\n\
}\n\
\n\
Animator instproc stopmaxtime { stoptime } {\n\
\n\
$self instvar maxtimeflag maxtime now mslider pipemode\n\
if {$pipemode == 0 } { return}\n\
\n\
if {$maxtimeflag == 1} {return}\n\
\n\
set maxtimeflag 1\n\
set maxtime $stoptime\n\
set now $stoptime\n\
$mslider setcurrenttime $now\n\
$mslider setpipemode 0\n\
}\n\
\n\
Animator instproc settime t {\n\
$self instvar sliderPressed range mintime timeSlider trace now \\\n\
maxtime graphs netViews statsViews maxtimeflag mslider pipemode\n\
$self instvar netModel\n\
$self tkvar nowDisp\n\
$self tkvar showData_ showRouting_ showMac_\n\
\n\
$netModel select-pkt $showData_  $showRouting_  $showMac_\n\
\n\
if { $t > $maxtime } {\n\
if {$pipemode == 0 } {\n\
$self stop 1\n\
return\n\
} else {\n\
if {$maxtimeflag == 1} {\n\
$self stop 1\n\
set range [expr $maxtime-$mintime]\n\
$mslider setmaxtime $maxtime \n\
set pipemode 0\n\
return\n\
} else {\n\
set maxtime [expr $maxtime+10.0]\n\
set range [expr $maxtime-$mintime]\n\
$mslider setmaxtime $maxtime \n\
}\n\
}\n\
\n\
} elseif { $t < $mintime } {\n\
set t $mintime\n\
}\n\
\n\
set now $t\n\
set nowDisp [format %.6f $now]\n\
\n\
\n\
if { $sliderPressed == 0 } {\n\
$self timesliderset $now\n\
}\n\
\n\
$self notifyObservers $now\n\
\n\
set event [$trace settime $now $sliderPressed]\n\
\n\
foreach graph $graphs {\n\
$graph settime $t\n\
}\n\
foreach netView $netViews {\n\
$self update_monitors $netView $now\n\
}\n\
$self update_annotations $now\n\
}\n\
\n\
\n\
Animator instproc slidetime { tick remote } {\n\
$self instvar now range mintime trace\n\
set now [expr ($tick * $range) / 1000. + $mintime]\n\
$self settime $now\n\
if { $remote } {\n\
$self peer_cmd 1 \"settime $now\"\n\
}\n\
}\n\
\n\
Animator instproc recordFrame {} {\n\
$self instvar netViews nam_record_filename nam_record_frame\n\
foreach netView $netViews {\n\
$netView record_frame [format \"$nam_record_filename%05d.xwd\" $nam_record_frame]\n\
incr nam_record_frame\n\
}\n\
}\n\
\n\
Animator instproc renderFrame { } {\n\
$self instvar running direction sliderPressed granularity \\\n\
pending_frame_\n\
$self tkvar nam_record_animation\n\
\n\
if { $running && !$sliderPressed } {\n\
$self nextFrame\n\
update idletasks\n\
if $nam_record_animation {\n\
$self recordFrame\n\
}\n\
set pending_frame_ [after $granularity \"$self renderFrame\"]\n\
}\n\
}\n\
\n\
Animator instproc redrawFrame {} {\n\
$self instvar now\n\
$self settime $now\n\
}\n\
\n\
Animator instproc remote_set_direction {t dir} {\n\
$self instvar direction\n\
$self settime $t\n\
set direction $dir\n\
}\n\
\n\
Animator instproc set_forward_dir remote {\n\
$self instvar direction now \n\
set direction 1\n\
if { $remote } {\n\
$self peer_cmd 1 \"remote_set_direction $now 1\"\n\
}\n\
}\n\
\n\
Animator instproc set_backward_dir remote {\n\
$self instvar direction now\n\
set direction -1\n\
if { $remote } {\n\
$self peer_cmd 1 \"remote_set_direction $now -1\"\n\
}\n\
}\n\
\n\
Animator instproc remote_play t {\n\
$self settime $t\n\
$self play 0\n\
}\n\
\n\
Animator instproc play {remote} {\n\
$self instvar running now\n\
set running 1\n\
after 0 \"$self renderFrame\"\n\
if { $remote } {\n\
$self peer_cmd 1 \"remote_play $now\"\n\
}\n\
}\n\
\n\
Animator instproc remote_stop t {\n\
$self stop 0\n\
$self settime $t\n\
}\n\
\n\
Animator instproc stop remote {\n\
$self instvar running now\n\
set running 0\n\
if { $remote } {\n\
$self peer_cmd 1 \"remote_stop $now\"\n\
}\n\
}\n\
\n\
Animator instproc reset { } {\n\
$self settime 0.\n\
$self peer_cmd 1 \"settime 0.\"\n\
}\n\
\n\
Animator instproc rewind { } {\n\
$self instvar now timeStep\n\
set t [expr $now - $timeStep*25.0]\n\
$self settime $t\n\
$self peer_cmd 1 \"settime $t\"\n\
}\n\
\n\
Animator instproc fast_fwd { } {\n\
$self instvar now timeStep\n\
set t [expr $now + $timeStep*25.0]\n\
$self settime $t\n\
$self peer_cmd 1 \"settime $t\"\n\
}\n\
\n\
Animator instproc next_event { } {\n\
$self instvar trace running\n\
set t [$trace nxtevent]\n\
$self settime $t\n\
$self peer_cmd 1 \"settime $t\"\n\
if { !$running } {\n\
$self nextFrame\n\
$self peer_cmd 1 nextFrame\n\
}\n\
}\n\
\n\
Animator instproc set_rate_ext { v remote } {\n\
$self instvar timeStep stepDisp rateSlider currRate\n\
set orig_v $v\n\
set timeStep [time2real $v]\n\
set v [expr 10.0 * log10($timeStep)]\n\
set stepDisp [step_format $timeStep]\n\
if { [$rateSlider get] != $v } { $rateSlider set $v }\n\
set currRate $v\n\
if { $remote } {\n\
$self peer_cmd 1 \"set_rate $orig_v 0\"\n\
}\n\
}\n\
\n\
Animator instproc set_rate { v remote } {\n\
$self instvar timeStep stepDisp rateSlider currRate\n\
set orig_v $v\n\
set timeStep [expr pow(10, $v / 10.)]\n\
set stepDisp [step_format $timeStep]\n\
if { [$rateSlider get] != $v } { $rateSlider set $v }\n\
set currRate $v\n\
if { $remote } {\n\
$self peer_cmd 1 \"set_rate $orig_v 0\"\n\
}\n\
}\n\
\n\
Animator instproc set_bcast_duration { time } {\n\
$self instvar netModel\n\
$netModel set bcast_duration_ $time\n\
}\n\
Animator instproc set_bcast_radius { time } {\n\
$self instvar netModel\n\
$netModel set bcast_radius_ $time\n\
}\n\
\n\
Animator instproc time_undo { } {\n\
$self instvar timeSlider prevTime now\n\
set currTime $now\n\
$self settime $prevTime\n\
$self peer_cmd 1 \"settime $prevTime\"\n\
set prevTime $currTime\n\
}\n\
\n\
Animator instproc rate_undo { } {\n\
$self instvar prevRate rateSlider\n\
set tmpRate [$rateSlider get]\n\
$self set_rate $prevRate 1\n\
$rateSlider set $prevRate\n\
set prevRate $tmpRate\n\
}\n\
\n\
\n\
Animator instproc auto_ff { } {\n\
$self tkvar nam_auto_ff\n\
set nam_auto_ff 1\n\
}\n\
\n\
Animator instproc speedup { t } {\n\
$self instvar running \n\
$self tkvar nam_auto_ff\n\
if { $nam_auto_ff && $running } {\n\
$self next_event \n\
}\n\
}\n\
\n\
Animator instproc normalspeed { } {\n\
}\n\
\n\
Animator instproc button_release_1 t {\n\
$self instvar timeSlider sliderPressed\n\
\n\
$self slidetime $t 1\n\
$timeSlider set $t\n\
set sliderPressed 0\n\
}\n\
\n\
Animator instproc button_press_1 s {\n\
$self instvar sliderPressed prevTime\n\
set sliderPressed 1\n\
set prevTime $s\n\
}\n\
\n\
Animator instproc displayStep args {\n\
$self instvar rateSlider stepDisp\n\
$rateSlider configure -label \"Step: $stepDisp\"\n\
}\n\
\n\
Animator instproc back_step { } {\n\
$self instvar running\n\
if $running { $self stop 1 }\n\
$self backFrame\n\
$self peer_cmd 1 backFrame\n\
}\n\
\n\
Animator instproc toggle_pause { } {\n\
$self instvar running\n\
if $running {\n\
$self stop 1\n\
} else {\n\
$self play 1\n\
}\n\
}\n\
\n\
Animator instproc single_step { } {\n\
$self instvar running\n\
if $running { $self stop 1 }\n\
$self nextFrame\n\
$self peer_cmd 1 nextFrame\n\
}\n\
\n\
Animator instproc dead name {\n\
$self instvar peers\n\
set i [lsearch -exact $peers $name]\n\
set peers [lreplace $peers $i $i]\n\
}\n\
\n\
Animator instproc destroy {} {\n\
[AnimControl instance] close-animator $self\n\
$self next\n\
}\n\
\n\
Animator instproc cleanup {} {\n\
if [$self is-sync] {\n\
$self remove-sync $self\n\
}\n\
$self instvar pending_frame_ tlw_ balloon_\n\
delete $balloon_\n\
destroy $tlw_\n\
if [info exists pending_frame_] {\n\
after cancel $pending_frame_\n\
}\n\
}\n\
\n\
Animator instproc done { } {\n\
$self peer_cmd 1 \"dead \\\"[tk appname]\\\"\"\n\
$self cleanup\n\
delete $self\n\
}\n\
\n\
Animator instproc all_done { } {\n\
$self peer_cmd 1 \"destroy\"\n\
$self cleanup\n\
delete $self\n\
}\n\
\n\
Animator instproc local_change_rate inc {\n\
$self instvar timeStep stepDisp\n\
if $inc {\n\
set timeStep [expr $timeStep + $timeStep*0.05]\n\
} else {\n\
set timeStep [expr $timeStep - $timeStep*0.05]\n\
}	    \n\
set stepDisp [step_format $timeStep]\n\
}\n\
\n\
Animator instproc change_rate inc {\n\
$self instvar timeStep\n\
$self local_change_rate $inc\n\
$self peer_cmd 1 \"local_change_rate $inc\"\n\
}\n\
\n\
Animator instproc ncolor {n0 color} {\n\
$self instvar netModel\n\
$netModel ncolor $n0 $color\n\
}\n\
\n\
Animator instproc ecolor {n0 n1 color} {\n\
$self instvar netModel\n\
$netModel ecolor $n0 $n1 $color\n\
$netModel ecolor $n1 $n0 $color\n\
}\n\
\n\
Animator instproc add-sync { anim } {\n\
Animator instvar SyncAnimator_\n\
lappend SyncAnimator_ $anim\n\
$self tkvar isSync_\n\
}\n\
\n\
Animator instproc remove-sync { anim } {\n\
Animator instvar SyncAnimator_\n\
set pos [lsearch $SyncAnimator_ $anim]\n\
if {$pos != -1} {\n\
set SyncAnimator_ [lreplace $SyncAnimator_ $pos $pos]\n\
}\n\
}\n\
\n\
Animator instproc is-sync {} {\n\
$self tkvar isSync_\n\
return $isSync_\n\
}\n\
\n\
Animator instproc get-sync {} {\n\
Animator instvar SyncAnimator_\n\
return $SyncAnimator_\n\
}\n\
\n\
Animator instproc get-name {} {\n\
$self instvar nam_name\n\
return $nam_name\n\
}\n\
\n\
Animator instproc get-tkwin {} {\n\
$self instvar tlw_\n\
return $tlw_\n\
}\n\
\n\
\n\
\n\
Animator instproc addObserver { o } {\n\
$self instvar observerlist_\n\
\n\
set cnt 0\n\
set oid [$o id]\n\
foreach ob $observerlist_ {\n\
set obid [$ob id]\n\
if { $oid == $obid } {\n\
set cnt 1\n\
break;\n\
} \n\
}   \n\
\n\
if { $cnt == 0 } {\n\
lappend observerlist_ $o\n\
}   \n\
}\n\
\n\
\n\
Animator instproc  deleteObserver { o } { \n\
\n\
$self instvar observerlist_\n\
set backlist_ \"\"\n\
set oid [$o id]\n\
foreach ob $observerlist_ {\n\
set obid [$ob id]\n\
if { $oid != $obid } {\n\
lappend backlist_ $ob\n\
} else {\n\
$o destory\n\
}\n\
}\n\
\n\
set observerlist_ $backlist_\n\
}\n\
\n\
Animator instproc notifyObservers { arg } {\n\
\n\
$self instvar observerlist_\n\
\n\
\n\
foreach ob $observerlist_ {\n\
\n\
if ![ catch { $ob info class } ] {\n\
\n\
$ob update $arg\n\
}\n\
}\n\
}\n\
\n\
Animator instproc show-energy {} {\n\
$self instvar showenergy_\n\
set showenergy_ 1\n\
}\n\
\n\
\n\
\n\
Animator instproc countObservers {} {\n\
$self instvar observerlist_\n\
set size [llength $observerlist_]\n\
return $size\n\
}\n\
\n\
\n\
\n\
Class BalloonHelp\n\
\n\
BalloonHelp instproc init { tlw } {\n\
$self instvar balloon_\n\
set balloon_ $tlw-bal\n\
toplevel $balloon_ -class Balloon -borderwidth 1 -relief groove \\\n\
-background black\n\
label $balloon_.info\n\
pack $balloon_.info -side left -fill y\n\
wm overrideredirect $balloon_ 1\n\
wm withdraw $balloon_\n\
}\n\
\n\
BalloonHelp instproc destroy {} {\n\
$self balloon_cancel\n\
}\n\
\n\
BalloonHelp instproc balloon_for {win mesg {delay 500}} {\n\
$self instvar balloon_ bhInfo_ balloon_delay_\n\
if {![info exists bhInfo_($win)]} {\n\
set bhInfo_($win) $mesg\n\
set balloon_delay_ $delay\n\
bind $win <Enter> \"$self balloon_pending %W\"\n\
bind $win <Leave> \"$self balloon_cancel\"\n\
} else {\n\
set bhInfo_($win) $mesg\n\
set balloon_delay_ $delay\n\
}\n\
}\n\
\n\
BalloonHelp instproc balloon_pending {win} {\n\
$self instvar bhInfo_ balloon_ balloon_delay_\n\
$self balloon_cancel\n\
set bhInfo_(pending) [after $balloon_delay_ \\\n\
\"$self balloon_show $win\"]\n\
}\n\
\n\
BalloonHelp instproc balloon_cancel {} {\n\
$self instvar bhInfo_ balloon_\n\
if [info exists bhInfo_(pending)] {\n\
after cancel $bhInfo_(pending)\n\
unset bhInfo_(pending)\n\
}\n\
wm withdraw $balloon_\n\
}\n\
\n\
BalloonHelp instproc balloon_show {win} {\n\
$self instvar bhInfo_ balloon_\n\
\n\
set bhInfo_(active) 1\n\
if { $bhInfo_(active) } {\n\
$balloon_.info configure -text $bhInfo_($win)\n\
set x [expr [winfo rootx $win]+[winfo width $win]+5]\n\
set y [expr [winfo rooty $win]+10]\n\
wm geometry $balloon_ +$x+$y\n\
wm deiconify $balloon_\n\
raise $balloon_\n\
}\n\
unset bhInfo_(pending)\n\
}\n\
\n\
\n\
Animator instproc take_snapshot {} {\n\
$self instvar netModel\n\
$self testsuite_view $netModel nam\n\
}\n\
\n\
Animator instproc testsuite_view { viewobject windowname } {\n\
puts \"take a snapshot here. \"\n\
$viewobject testview testview\n\
}\n\
\n\
Animator instproc playing_backward {} {\n\
$self instvar netView running backward\n\
focus $netView\n\
if { $backward == 0 } {\n\
$self set_backward_dir 1\n\
if { $running != 1 } {\n\
$self play 1\n\
$self renderFrame\n\
}\n\
}\n\
set backward 1\n\
}\n\
\n\
Animator instproc playing_forward {} {\n\
$self instvar netView running direction\n\
focus $netView\n\
if { $direction == -1} {\n\
$self set_forward_dir 1\n\
if { $running != 1 } {\n\
$self play 1\n\
$self renderFrame\n\
}\n\
}\n\
}\n\
\n\
Animator instproc terminating_nam {} {\n\
destroy .\n\
}\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
NetworkModel/Auto set RANDOM_SEED_ 1\n\
NetworkModel/Auto set KCa_ 0.15\n\
NetworkModel/Auto set KCr_ 0.75\n\
NetworkModel/Auto set Recalc_ 1\n\
NetworkModel/Auto set AUTO_ITERATIONS_ 10\n\
NetworkModel/Auto set INCR_ITERATIONS_ 10\n\
\n\
NetworkModel/Auto instproc init { animator tracefile } {\n\
eval $self next $animator $tracefile\n\
NetworkModel/Auto instvar INCR_ITERATIONS_ KCa_ KCr_\n\
$animator set-layout-params $INCR_ITERATIONS_ $KCa_ $KCr_\n\
}\n\
\n\
NetworkModel/Auto instproc do_relayout { Iter Kca Kcr Recalc } {\n\
$self instvar INCR_ITERATIONS_ KCa_ KCr_ Recalc_\n\
set KCa_ $Kca\n\
set KCr_ $Kcr\n\
set Recalc_ $Recalc\n\
set INCR_ITERATIONS_ $Iter\n\
$self relayout\n\
}\n\
\n\
Animator instproc popup_annotation { w x y } {\n\
$self instvar annoBox running \n\
$self stop 1\n\
catch {destroy $w.menuPopa}\n\
menu $w.menuPopa -relief groove -borderwidth 2\n\
$w.menuPopa add command -label \"Command\"\n\
$w.menuPopa add separator\n\
$w.menuPopa add command -label \"Add\" \\\n\
-command \"$self cmdAnnotationAdd $w $x $y\"\n\
set index [$annoBox nearest $y]\n\
$w.menuPopa add command -label \"Delete\" \\\n\
-command \"$self cmdAnnotationDelete $index\"\n\
$w.menuPopa add command -label \"Info\" \\\n\
-command \"$self cmdAnnotationInfo $w $index $x $y\"\n\
set rx [winfo rootx $w]\n\
set ry [winfo rooty $w]\n\
set rx [expr $rx + $x]\n\
set ry [expr $ry + $y]\n\
tk_popup $w.menuPopa $rx $ry\n\
}\n\
\n\
Animator instproc cmdAnnotationAdd { w x y } {\n\
$self instvar annoInputText now\n\
\n\
catch { destroy .inputBox }\n\
set f [smallfont]\n\
toplevel .inputBox\n\
label .inputBox.title -text \"New Annotation Text\" -font $f\n\
set annoInputText \"\"\n\
entry .inputBox.input -width 40 -relief sunken \\\n\
-textvariable annoInputText\n\
button .inputBox.done -text \"Done\" -borderwidth 2 -relief raised \\\n\
-font $f \\\n\
-command \"add_annotation $now; catch {destroy .inputBox}\"\n\
pack .inputBox.title -side top\n\
pack .inputBox.input -side top\n\
pack .inputBox.done -side top\n\
\n\
bind .inputBox.input <Return> \\\n\
\"add_annotation $now; catch { destroy .inputBox }\"\n\
}\n\
\n\
Animator instproc cmdAnnotationDelete { index } {\n\
$self instvar annoBox annoTimeList\n\
$annoBox delete $index\n\
set annoTimeList [lreplace $annoTimeList $index $index]\n\
}\n\
\n\
Animator instproc cmdAnnotationInfo { w index x y } {\n\
$self instvar annoBox annoTimeList\n\
set str [$annoBox get $index]\n\
set lst [list \"At time\" [lindex $annoTimeList $index] \", $str\"]\n\
set str [join $lst]\n\
\n\
catch { destroy $w.mBox }\n\
set f [smallfont]\n\
frame $w.mBox -relief groove -borderwidth 2\n\
label $w.mBox.message -text $str -font $f\n\
button $w.mBox.done -text \"Done\" -borderwidth 2 -relief raised \\\n\
-font $f \\\n\
-command \"catch { destroy $w.mBox }\"\n\
pack $w.mBox.message\n\
pack $w.mBox.done\n\
place_frame $w $w.mBox $x $y\n\
}\n\
\n\
Animator instproc add_annotation { time } {\n\
$self instvar annoBox annoTimeList annoInputText\n\
\n\
set size [$annoBox size]\n\
for {set index 0} {$index < $size} {incr index} {\n\
if {[lindex $annoTimeList $index] > $time} {\n\
break\n\
}\n\
}\n\
\n\
if {$index == $size} {\n\
set index \"end\"\n\
}\n\
$annoBox insert $index \"$annoInputText\"\n\
if {$size == 0} {\n\
lappend annoTimeList $time\n\
} else {\n\
set annoTimeList [linsert $annoTimeList $index $time]\n\
}\n\
$annoBox see $index\n\
}\n\
\n\
Animator instproc sim_annotation { time seq args } {\n\
$self instvar annoBox annoTimeList annoBoxHeight regAnno\n\
\n\
if ![info exists regAnno($seq)] {\n\
$annoBox insert end \"$args\"\n\
set size [$annoBox size]\n\
set annoTimeList [lappend annoTimeList $time]\n\
set regAnno($seq) 1\n\
set d [expr $size-$annoBoxHeight]\n\
if {$d > 0} {\n\
$annoBox yview $d\n\
}\n\
}\n\
}\n\
\n\
Animator instproc update_annotations { time } {\n\
$self instvar annoBox annoTimeList annoBoxHeight direction annoJump\n\
\n\
set size [$annoBox size]\n\
if {($size == 0) || ($annoJump == 1)} {\n\
set annoJump 0\n\
return \n\
}\n\
set i 0\n\
foreach l $annoTimeList {\n\
if {$l > $time} {\n\
break\n\
} else {\n\
incr i\n\
}\n\
}\n\
if {$direction != 1 && $i != 0} {\n\
incr i -1\n\
}\n\
if {$i < $annoBoxHeight/2} {\n\
set d 0\n\
} else {\n\
set d [expr $i - $annoBoxHeight / 2]\n\
}\n\
$annoBox yview $d\n\
$annoBox selection clear 0 $size\n\
$annoBox selection set $i $i\n\
}\n\
\n\
Animator instproc jump_to_annotation { anno } {\n\
$self instvar annoTimeList annoJump\n\
set idx [$anno index active]\n\
set annoJump 1\n\
$self settime [lindex $annoTimeList $idx]\n\
$self peer_cmd 1 \"settime [lindex $annoTimeList $idx]\"\n\
}\n\
\n\
Animator instproc link-up {now src dst} {\n\
$self sim_annotation $now link($src:$dst) up\n\
}\n\
\n\
Animator instproc link-down {now src dst} {\n\
$self sim_annotation $now link($src:$dst) down\n\
}\n\
\n\
Animator instproc node-up {now src} {\n\
$self sim_annotation $now $node $src up\n\
}\n\
\n\
Animator instproc node-down {now src} {\n\
$self sim_annotation $now $node $src down\n\
}\n\
\n\
\n\
Animator instproc node_tclscript {node_id button_label tcl_command} {\n\
$self instvar netModel\n\
$netModel set_node_tclscript $node_id $button_label $tcl_command\n\
}\n\
\n\
NetworkModel set bcast_duration_ 0.01\n\
NetworkModel set bcast_radius_ 250\n\
\n\
NetworkModel instproc init {animator tracefile} {\n\
$self instvar showenergy_\n\
$self next $animator	;# create shadow, etc.\n\
\n\
if {$tracefile != \" \"} {\n\
$self nam_addressing $tracefile\n\
$self nam_layout $tracefile\n\
$self layout\n\
\n\
if [info exists showenergy_] {\n\
if {$showenergy_ == 1} {\n\
$animator show-energy\n\
}\n\
}\n\
}\n\
}\n\
\n\
NetworkModel instproc nam_addressing { tracefile } {\n\
set stream [new NamStream $tracefile]\n\
set time \"0\"\n\
while {([$stream eof]==0)&&([string compare $time \"*\"]!=0)} {\n\
set line [$stream gets]\n\
set time [get_trace_item \"-t\" $line]\n\
}\n\
\n\
$self instvar netAddress\n\
if ![info exists netAddress] {\n\
set netAddress [new Address]\n\
}\n\
while {([$stream eof]==0)&&([string compare $time \"*\"]==0)} {\n\
set cmd [lindex $line 0]\n\
set time [get_trace_item \"-t\" $line]\n\
if {[string compare $time \"*\"]!=0} {break}\n\
switch $cmd {\n\
\"A\" {\n\
set hier [get_trace_item \"-n\" $line]\n\
if {$hier != \"\"} {\n\
set ps [get_trace_item \"-p\" $line]\n\
set pm [get_trace_item \"-o\" $line]\n\
set ms [get_trace_item \"-c\" $line]\n\
set mm [get_trace_item \"-a\" $line]\n\
$netAddress portbits-are $ps $pm\n\
$netAddress mcastbits-are $ms $mm\n\
} else {\n\
set hier [get_trace_item \"-h\" $line]\n\
set nh [get_trace_item \"-m\" $line]\n\
set nm [get_trace_item \"-s\" $line]\n\
$netAddress add-hier $hier $nh $nm\n\
}\n\
}\n\
}\n\
set line [$stream gets]\n\
}\n\
$stream close\n\
}\n\
\n\
NetworkModel instproc nam_layout { tracefile } {\n\
set stream [new NamStream $tracefile]\n\
set time \"0\"\n\
\n\
while {([$stream eof]==0)&&([string compare $time \"*\"] != 0)} {\n\
set line [$stream gets]\n\
set time [get_trace_item \"-t\" $line]\n\
}\n\
\n\
while {([$stream eof]==0)&&([string compare $time \"*\"] == 0)} {\n\
set cmd [lindex $line 0]\n\
set time [get_trace_item \"-t\" $line]\n\
if {[string compare $time \"*\"] != 0} {\n\
break\n\
}\n\
\n\
switch $cmd {\n\
\"n\" {\n\
$self node $line\n\
}\n\
\"g\" {\n\
$self layout_agent [lrange $line 1 end]\n\
}\n\
\"l\" {\n\
$self layout_link [lrange $line 1 end]\n\
}\n\
\"L\" {\n\
$self layout_lanlink [lrange $line 1 end]\n\
}\n\
\"X\" {\n\
$self layout_lan [lrange $line 1 end]\n\
}\n\
\"q\" {\n\
$self layout_queue [lrange $line 1 end]\n\
}\n\
\"c\" {\n\
$self layout_color [lrange $line 1 end]\n\
}\n\
}\n\
set line [$stream gets]\n\
}\n\
$stream close\n\
}\n\
\n\
NetworkModel instproc layout_node {line} {\n\
$self instvar showenergy_\n\
\n\
set color black\n\
set lcolor black\n\
set shape circle\n\
set dlabel \"\"\n\
set src \"\"\n\
set size 0\n\
set addr 0\n\
\n\
for {set i 0} {$i < [llength $line]} {incr i} {\n\
set item [string range [lindex $line $i] 1 end]\n\
switch $item {\n\
\"s\" {\n\
set src [lindex $line [expr $i+1]]\n\
}\n\
\"v\" {\n\
set shape [lindex $line [expr $i+1]]\n\
}\n\
\"c\" {\n\
set color [lindex $line [expr $i+1]]\n\
if {[string range $color 0 0] == \"\\#\"} {\n\
set rgb [winfo rgb . $color]\n\
set color \"[$self lookupColorName [lindex $rgb 0] \\\n\
[lindex $rgb 1] [lindex $rgb 2]]\"\n\
}\n\
if [info exists showenergy_] {\n\
if { $color == \"green\"} {\n\
set showenergy_ 1\n\
}\n\
}\n\
}\n\
\"z\" {\n\
set size [lindex $line [expr $i+1]]\n\
}\n\
\"a\" {\n\
set addr [lindex $line [expr $i+1]]\n\
}\n\
\"x\" {\n\
set xcor [lindex $line [expr $i+1]]\n\
set showenergy_ 0\n\
}\n\
\"y\" {\n\
set ycor [lindex $line [expr $i+1]]\n\
set showenergy_ 0\n\
}\n\
\"i\" {\n\
set lcolor [lindex $line [expr $i+1]]\n\
if {[string range $lcolor 0 0] == \"\\#\"} {\n\
set rgb [winfo rgb . $lcolor]\n\
set lcolor \"[$self lookupColorName [lindex $rgb 0] \\\n\
[lindex $rgb 1] [lindex $rgb 2]]\"\n\
}\n\
}\n\
\"b\" {\n\
set dlabel [lindex $line [expr $i+1]]\n\
}\n\
}\n\
\n\
if {[string range $item 0 0]==\"-\"} {\n\
incr i\n\
}\n\
}\n\
if {$src==\"\"} {\n\
puts \"Error in parsing tracefile line:\\n$line\\n\"\n\
puts \"No -s <node id> was given.\"\n\
exit\n\
}\n\
\n\
if [info exists xcor] {\n\
if {$dlabel != \"\"} {\n\
$self node $src $shape $color $addr $size $lcolor \\\n\
$xcor $ycor $dlabel \n\
} else {\n\
$self node $src $shape $color $addr $size $lcolor \\\n\
$xcor $ycor\n\
}\n\
} else {\n\
if {$dlabel != \"\"} {\n\
$self node $src $shape $color $addr $size $lcolor $dlabel\n\
} else {\n\
$self node $src $shape $color $addr $size $lcolor\n\
}\n\
}\n\
}\n\
\n\
NetworkModel instproc layout_agent {line} {\n\
set label \"\"\n\
set role \"\"\n\
set node \"\"\n\
set flowcolor black\n\
set winInit \"\"\n\
set win \"\"\n\
set maxcwnd \"\"\n\
set tracevar \"\"\n\
set start \"\"\n\
set producemore \"\"\n\
set stop  \"\"\n\
set packetsize \"\"\n\
set interval \"\"\n\
set number \"\"\n\
set partner \"\"\n\
\n\
for {set i 0} {$i < [llength $line]} {incr i} {\n\
set item [string range [lindex $line $i] 1 end]\n\
switch $item {\n\
\"l\" {\n\
set label [lindex $line [expr $i+1]]\n\
}\n\
\"r\" {\n\
set role [lindex $line [expr $i+1]]\n\
}\n\
\"s\" {\n\
set node [lindex $line [expr $i+1]]\n\
}\n\
\"c\" {\n\
set flowcolor [lindex $line [expr $i+1]]\n\
}\n\
\"i\" {\n\
set winInit [lindex $line [expr $i+1]]\n\
}\n\
\"w\" {\n\
set win [lindex $line [expr $i+1]]\n\
}\n\
\"m\" {\n\
set maxcwnd [lindex $line [expr $i+1]]\n\
}\n\
\"t\" {\n\
set tracevar [lindex $line [expr $i+1]]\n\
}\n\
\"b\" {\n\
set start [lindex $line [expr $i+1]]\n\
}\n\
\"o\" {\n\
set stop [lindex $line [expr $i+1]]\n\
}\n\
\"p\" {\n\
set producemore [lindex $line [expr $i+1]]\n\
}\n\
\"k\" {\n\
set packetsize [lindex $line [expr $i+1]]\n\
}\n\
\"v\" {\n\
set interval [lindex $line [expr $i+1]]\n\
}\n\
\"n\" {\n\
set number [lindex $line [expr $i+1]]\n\
}\n\
\"u\" {\n\
set partner [lindex $line [expr $i+1]]\n\
}\n\
}\n\
if {[string range $item 0 0]==\"-\"} {incr i}\n\
}\n\
if {$label==\"\"} {\n\
puts \"Error in parsing tracefile line:\\n$line\\n\"\n\
exit\n\
}\n\
\n\
if {[string compare $role \"20\"]==0} {\n\
$self agent $label $role $node $number $partner\n\
\n\
} elseif {[string compare $role \"10\"]==0 && [string compare $label \"CBR\"]==0} {\n\
$self agent $label $role $node $flowcolor $packetsize $interval $start $stop $number $partner\n\
\n\
} else {\n\
$self agent $label $role $node $flowcolor $winInit $win $maxcwnd $tracevar $start $producemore $stop $number $partner\n\
}\n\
}\n\
\n\
\n\
NetworkModel instproc layout_lan {line} {\n\
set name \"\"\n\
set rate \"\"\n\
set delay \"\"\n\
set orient down\n\
for {set i 0} {$i < [llength $line]} {incr i} {\n\
set item [string range [lindex $line $i] 1 end]\n\
\n\
switch $item {\n\
\"n\" {\n\
set name [lindex $line [expr $i+1]]\n\
}\n\
\"r\" {\n\
set rate [lindex $line [expr $i+1]]\n\
}\n\
\"D\" {\n\
set delay [lindex $line [expr $i+1]]\n\
}\n\
\"o\" {\n\
set orient [lindex $line [expr $i+1]]\n\
}\n\
}\n\
\n\
if {[string range $item 0 0]==\"-\"} {\n\
incr i\n\
}\n\
}\n\
\n\
if {$name==\"\"} {\n\
puts \"netModel.tcl found an Error in parsing tracefile line:\\n$line\\n\"\n\
exit\n\
}\n\
\n\
set th [nam_angle $orient]\n\
\n\
set rate [expr [bw2real $rate] / 1000000.0]\n\
set delay [expr [time2real $delay] * 1000.0]\n\
\n\
$self lan $name [bw2real $rate] [time2real $delay] [expr $th]\n\
}\n\
\n\
NetworkModel instproc layout_link {line} {\n\
set orient right\n\
set src \"\"\n\
set dst \"\"\n\
set rate 10Mb\n\
set delay 10ms\n\
set color \"\"\n\
set dlabel \"\"\n\
set length 0\n\
for {set i 0} {$i < [llength $line]} {incr i} {\n\
set item [string range [lindex $line $i] 1 end]\n\
switch \"$item\" {\n\
\"s\" {\n\
set src [lindex $line [expr $i+1]]\n\
}\n\
\"d\" {\n\
set dst [lindex $line [expr $i+1]]\n\
}\n\
\"r\" {\n\
set rate [lindex $line [expr $i+1]]\n\
}\n\
\"D\" {\n\
set delay [lindex $line [expr $i+1]]\n\
}\n\
\"h\" {\n\
set length [lindex $line [expr $i+1]]\n\
}\n\
\"O\" {\n\
set orient [lindex $line [expr $i+1]]\n\
}\n\
\"o\" {\n\
set orient [lindex $line [expr $i+1]]\n\
}\n\
\"c\" {\n\
set color [lindex $line [expr $i+1]]\n\
}\n\
\"b\" {\n\
set dlabel [lindex $line [expr $i+1]]\n\
}\n\
}\n\
if {[string range $item 0 0]==\"-\"} {\n\
incr i\n\
}\n\
}\n\
\n\
if {($src==\"\")||($dst==\"\")} {\n\
puts \"Error in parsing tracefile line:\\n$line\\n\"\n\
exit\n\
}\n\
set th [nam_angle $orient]\n\
set rev [expr $th + 1]\n\
if { $rev > 2.0 } {\n\
set rev [expr $rev - 2.0]\n\
}\n\
\n\
set rate [expr [bw2real $rate] / 1000000.0]\n\
set delay [expr [time2real $delay] * 1000.0]\n\
\n\
$self link $dst $src $rate $delay $length $rev\n\
\n\
$self link $src $dst $rate $delay $length $th\n\
\n\
if { $color!=\"\" } {\n\
$self ecolor $src $dst $color \n\
$self ecolor $dst $src $color \n\
}\n\
\n\
if { $dlabel!=\"\" } {\n\
$self edlabel $src $dst $dlabel\n\
$self edlabel $dst $src $dlabel\n\
}\n\
}\n\
\n\
NetworkModel instproc layout_lanlink {line} {\n\
set orient right\n\
set src \"\"\n\
set dst \"\"\n\
for {set i 0} {$i < [llength $line]} {incr i} {\n\
set item [string range [lindex $line $i] 1 end]\n\
switch \"$item\" {\n\
\"s\" {\n\
set src [lindex $line [expr $i+1]]\n\
}\n\
\"d\" {\n\
set dst [lindex $line [expr $i+1]]\n\
}\n\
\"O\" {\n\
set orient [lindex $line [expr $i+1]]\n\
}\n\
\"o\" {\n\
set orient [lindex $line [expr $i+1]]\n\
}\n\
}\n\
\n\
if {[string range $item 0 0]==\"-\"} {\n\
incr i\n\
}\n\
}\n\
\n\
if {($src==\"\")||($dst==\"\")} {\n\
puts \"Error in parsing tracefile line no source or destination in lanlink:\\n$line\\n\"\n\
exit\n\
}\n\
\n\
set th [nam_angle $orient] ;# global function\n\
\n\
$self lanlink $dst $src $th\n\
}\n\
\n\
NetworkModel instproc layout_queue {line} {\n\
for {set i 0} {$i < [llength $line]} {incr i} {\n\
set item [string range [lindex $line $i] 1 end]\n\
switch $item {\n\
\"s\" {\n\
set src [lindex $line [expr $i+1]]\n\
}\n\
\"d\" {\n\
set dst [lindex $line [expr $i+1]]\n\
}\n\
\"a\" {\n\
set angle [lindex $line [expr $i+1]]\n\
}\n\
}\n\
if {[string range $item 0 0]==\"-\"} {incr i}\n\
}\n\
$self queue $src $dst $angle\n\
}\n\
\n\
NetworkModel instproc layout_color {line} {\n\
for {set i 0} {$i < [llength $line]} {incr i} {\n\
set item [string range [lindex $line $i] 1 end]\n\
switch $item {\n\
\"i\" {\n\
set ix [lindex $line [expr $i+1]]\n\
}\n\
\"n\" {\n\
set name [lindex $line [expr $i+1]]\n\
if {[string range $name 0 0] == \"\\#\"} {\n\
set rgb [winfo rgb . $name]\n\
set name \"[$self lookupColorName [lindex $rgb 0] \\\n\
[lindex $rgb 1] [lindex $rgb 2]]\"\n\
}\n\
\n\
}\n\
}\n\
if {[string range $item 0 0]==\"-\"} {incr i}\n\
}\n\
$self color $ix $name\n\
}\n\
\n\
NetworkModel instproc traffic_filter {type} {\n\
$self select-traffic $type\n\
}\n\
\n\
NetworkModel instproc src_filter {src} {\n\
$self select-src $src\n\
}\n\
\n\
NetworkModel instproc dst_filter {dst} {\n\
$self select-dst $dst\n\
}\n\
\n\
NetworkModel instproc fid_filter {fid} {\n\
$self select-fid $fid\n\
}\n\
\n\
\n\
proc filesel {w dlg args} {\n\
frame $w -borderwidth 0\n\
set next \"\"\n\
set ctr 0\n\
foreach arg $args {\n\
if {[string range $arg 0 0]==\"-\"} {\n\
switch [string range $arg 1 end] {\n\
\"variable\" {\n\
set next variable\n\
}\n\
\"command\" {\n\
set next command\n\
}\n\
\"label\" {\n\
set next label\n\
}\n\
}\n\
} else {\n\
if {$next!=\"\"} {\n\
set vars($next) $arg\n\
}\n\
}\n\
}\n\
global $vars(variable)\n\
set $vars(variable) [pwd]\n\
frame $w.f0 -relief sunken -borderwidth 2\n\
listbox $w.f0.lb -width 50 -height 10 \\\n\
-yscroll \"$w.f0.sb set\" \\\n\
-selectforeground [resource activeForeground] \\\n\
-selectbackground [resource activeBackground] \\\n\
-highlightthickness 0\n\
scrollbar $w.f0.sb -command \"$w.f0.lb yview\" \\\n\
-highlightthickness 0\n\
pack $w.f0.lb -side left\n\
pack $w.f0.sb -side left -fill y\n\
pack $w.f0 -side top\n\
\n\
label $w.l2 -text $vars(label)\n\
pack $w.l2 -side top -anchor w\n\
\n\
entry $w.entry -width 50 -relief sunken\n\
bind $w.entry <Return> \\\n\
\"checkfile \\[set $vars(variable)\\] \\\"w\\\" $w $dlg \\\"$vars(command)\\\"\"\n\
$w.entry insert 0 [pwd]\n\
bind $w.entry <Delete> \\\n\
\"[bind Entry <BackSpace>];\\\n\
set $vars(variable) \\[$w.entry get\\];break\"\n\
bind $w.entry <BackSpace> \\\n\
\"[bind Entry <BackSpace>];\\\n\
set $vars(variable) \\[$w.entry get\\];break\"\n\
bind $w.entry <Key> \\\n\
\"[bind Entry <Key>];\\\n\
set $vars(variable) \\[$w.entry get\\];break\"\n\
bind $w.f0.lb <1> \"%W selection set \\[%W nearest %y\\];\\\n\
$w.entry delete 0 end;\\\n\
set $vars(variable) \\[pwd\\]/\\[%W get \\[%W nearest %y\\]\\]; \\\n\
$w.entry insert 0 \\[pwd\\]/\\[%W get \\[%W nearest %y\\]\\]\"\n\
bind $w.f0.lb <Double-Button-1> \\\n\
\"checkfile \\[set $vars(variable)\\] \\\"w\\\" $w $dlg \\\"$vars(command)\\\"\"\n\
pack $w.entry -side top -anchor w -fill x -expand true\n\
\n\
checkfile [pwd] \"r\" $w $dlg \"\"\n\
return $w\n\
}\n\
\n\
proc checkfile {file op w dlg cmd} {\n\
if {[file isdirectory $file]} {\n\
$w.entry delete 0 end\n\
cd $file\n\
$w.entry insert 0 [pwd]\n\
$w.f0.lb delete 0 end\n\
$w.f0.lb insert end ..\n\
foreach i [glob *] {\n\
$w.f0.lb insert end $i\n\
}\n\
} elseif {[file isfile $file]} {\n\
if {$op == \"r\"} {\n\
set flag [file readable $file]\n\
} else {\n\
set flag [file writable $file]\n\
}\n\
if $flag {\n\
eval $cmd\n\
destroy $dlg\n\
} else {\n\
errorpopup \"Error\" \"File $file is not writable\"\n\
}\n\
} else {\n\
set dirname [file dirname $file]\n\
set fname [file tail $file]\n\
if {$dirname != \"\"} {\n\
if [catch \"cd $dirname\"] {\n\
eval $cmd\n\
destroy $dlg\n\
}\n\
}\n\
set flist [glob -nocomplain $fname]\n\
$w.entry delete 0 end\n\
$w.entry insert 0 [pwd]\n\
$w.f0.lb delete 0 end\n\
if [llength $flist] {\n\
$w.f0.lb insert end ..\n\
foreach i $flist {\n\
$w.f0.lb insert end $i\n\
}\n\
} else {\n\
if {$op == \"w\"} {\n\
eval $cmd\n\
destroy $dlg\n\
} else {\n\
errorpopup \"Error\" \"File $file is not writable\"\n\
}\n\
}\n\
}\n\
}\n\
\n\
\n\
Animator instproc save_layout {} {\n\
$self instvar netModel tlw_\n\
$self tkvar savefile\n\
\n\
toplevel $tlw_.save\n\
set w $tlw_.save\n\
\n\
wm title $w \"Save layout\"\n\
frame $w.f -borderwidth 1 -relief raised\n\
pack $w.f -side top -fill both -expand true\n\
filesel $w.f.sel $w -variable [$self tkvarname savefile] \\\n\
-command \"$netModel savelayout \\[set [$self tkvarname savefile]\\]\" \\\n\
-label \"File to save to:\"\n\
pack $w.f.sel -side top -fill both -expand true\n\
frame $w.f.btns -borderwidth 0\n\
pack $w.f.btns -side top -fill x\n\
button $w.f.btns.save -text Save \\\n\
-command \"$self do_save_layout $w.f.sel $w\"\n\
pack $w.f.btns.save -side left -fill x -expand true\n\
button $w.f.btns.cancel -text Cancel -command \"destroy $w\"\n\
pack $w.f.btns.cancel -side left -fill x -expand true\n\
set savefile [pwd]\n\
}\n\
\n\
Animator instproc do_save_layout {w dlg} {\n\
$self instvar netModel\n\
$self tkvar savefile\n\
set t $savefile\n\
puts \"save to file $t\"\n\
checkfile $t \"w\" $w $dlg \"$netModel savelayout $t\"\n\
}\n\
\n\
Animator instproc disablefile {w} {\n\
$w.entry configure -state disabled \\\n\
-foreground [option get . disabledForeground Nam]\n\
$w.l2 configure -foreground [option get . disabledForeground Nam]\n\
$w.f0.lb configure -foreground [option get . disabledForeground Nam]\n\
}\n\
\n\
Animator instproc normalfile {w filevar} {\n\
$self tkvar $filevar\n\
$w.entry configure -state normal \\\n\
-foreground [option get . foreground Nam]\n\
set $filevar [$w.entry get]\n\
$w.l2 configure -foreground [option get . foreground Nam]\n\
$w.f0.lb configure -foreground [option get . foreground Nam]\n\
}\n\
\n\
Animator instproc print_view { viewobject windowname } {\n\
$self tkvar printfile printto \n\
$self instvar tlw_	\n\
\n\
set p print\n\
\n\
if {[winfo exists $tlw_.$windowname-$p]} {\n\
puts \"Quit due to duplicate print view window!\"\n\
}\n\
\n\
\n\
toplevel $tlw_.$windowname-$p\n\
set w $tlw_.$windowname-$p\n\
wm title $w \"Print $windowname\"    \n\
\n\
frame $w.f -borderwidth 1 -relief raised\n\
pack $w.f -side top -fill both -expand true\n\
frame $w.f.f0 \n\
pack $w.f.f0 -side top -fill x\n\
label $w.f.f0.l -text \"Print to:\"\n\
pack $w.f.f0.l -side left\n\
\n\
$self tkvar printto\n\
radiobutton $w.f.f0.r1 -text \"Printer\" -variable [$self tkvarname printto] \\\n\
-value printer -command \"$self disablefile $w.f.sel;\\\n\
$w.f.f1.e configure -state normal -foreground \\\n\
[option get . foreground Nam];\\\n\
$w.f.f1.l configure -foreground [option get . foreground Nam];\\\n\
set [$self tkvarname printfile] \\[$w.f.f1.e get\\]\"\n\
pack $w.f.f0.r1 -side left \n\
radiobutton $w.f.f0.r2 -text \"File\" -variable [$self tkvarname printto] \\\n\
-value file -command \"$self normalfile $w.f.sel printfile;\\\n\
$w.f.f1.e configure -state disabled -foreground \\\n\
[option get . disabledForeground Nam];\\\n\
$w.f.f1.l configure -foreground \\\n\
[option get . disabledForeground Nam]\"\n\
pack $w.f.f0.r2 -side left \n\
frame $w.f.f1 \n\
pack $w.f.f1 -side top -fill x\n\
label $w.f.f1.l -text \"Print command:\"\n\
pack $w.f.f1.l -side left\n\
entry $w.f.f1.e \n\
pack $w.f.f1.e -side left -fill x -expand true\n\
bind $w.f.f1.e <Key> \"[bind Entry <Key>];\\\n\
set [$self tkvarname printfile] \\[$w.f.f1.e get\\];break\"\n\
bind $w.f.f1.e <Return> \\\n\
\"$self print_animation $viewobject $w\"\n\
set printer \"\"\n\
global env\n\
catch {set printer $env(PRINTER)}\n\
if {$printer!=\"\"} {\n\
$w.f.f1.e insert 0 \"lpr -P$printer \"\n\
} else {\n\
$w.f.f1.e insert 0 \"lpr \"\n\
}\n\
filesel $w.f.sel $w -variable [$self tkvarname printfile] \\\n\
-command \"$viewobject psview \\[set [$self tkvarname printfile]\\]\" \\\n\
-label \"File to save to:\"\n\
pack $w.f.sel -side top -fill both -expand true\n\
frame $w.f.btns -borderwidth 0\n\
pack $w.f.btns -side top -fill x\n\
button $w.f.btns.print -text Print \\\n\
-command \"$self print_animation $viewobject $w\"\n\
pack $w.f.btns.print -side left -fill x -expand true\n\
button $w.f.btns.cancel -text Cancel -command \"destroy $w\"\n\
pack $w.f.btns.cancel -side left -fill x -expand true\n\
\n\
set printto printer\n\
set printfile [$w.f.f1.e get]\n\
$self disablefile $w.f.sel\n\
}\n\
\n\
Animator instproc print_animation {viewobject w} {\n\
$self tkvar printto printfile\n\
\n\
if {$printto==\"file\"} {\n\
checkfile $printfile \"w\" $w.f.sel $w \\\n\
\"$viewobject psview $printfile\"\n\
} else {\n\
$viewobject psview /tmp/nam.ps\n\
puts [eval \"exec $printfile /tmp/nam.ps\"]\n\
catch {destroy $w}\n\
}\n\
}\n\
\n\
Animator instproc show_subtrees {} {\n\
$self instvar netModel\n\
$netModel showtrees\n\
}\n\
\n\
Animator instproc new_view {} {\n\
$self instvar netModel viewctr nam_name netViews tlw_\n\
\n\
toplevel $tlw_.v$viewctr\n\
set w $tlw_.v$viewctr\n\
\n\
incr viewctr\n\
wm title $w $nam_name\n\
frame $w.f\n\
frame $w.f.f -borderwidth 2 -relief sunken\n\
$netModel view $w.f.f.net\n\
set newView $w.f.f.net\n\
lappend netViews $newView\n\
pack $w.f.f.net -side top -expand true -fill both\n\
\n\
$newView xscroll $w.f.hsb\n\
scrollbar $w.f.hsb -orient horizontal -width 10 -borderwidth 1 \\\n\
-command \"$newView xview\"\n\
$w.f.hsb set 0.0 1.0\n\
pack $w.f.hsb -side bottom -fill x\n\
pack $w.f.f -side top -fill both -expand true\n\
\n\
frame $w.f2\n\
$newView yscroll $w.f2.vsb\n\
scrollbar $w.f2.vsb -orient vertical -width 10 -borderwidth 1 \\\n\
-command \"$newView yview\"\n\
$w.f2.vsb set 0.0 1.0\n\
pack $w.f2.vsb -side top -fill y -expand true\n\
frame $w.f2.l -width 12 -height 12\n\
pack $w.f2.l -side top\n\
\n\
frame $w.ctrl -borderwidth 2 -relief groove\n\
$self build-zoombar $newView $w.ctrl $w\n\
pack $w.ctrl -side left -fill y\n\
\n\
pack $w.f2 -side right -fill y\n\
pack $w.f -side left -fill both -expand true\n\
$self window_bind $w\n\
$self view_bind $newView\n\
}\n\
\n\
Animator instproc energy_view {} {\n\
$self instvar netView now vslider windows nam_name graphs tlw_ tracefile\n\
\n\
set name \"node_energy\"\n\
\n\
set graphtrace [new Trace $tracefile $self]\n\
set netgraph [new EnergyNetworkGraph]\n\
$windows(title) configure -text \"Please wait - reading tracefile...\"\n\
update\n\
set maxtime [$graphtrace maxtime]\n\
set mintime [$graphtrace mintime]\n\
$graphtrace connect $netgraph\n\
$netgraph timerange $mintime $maxtime\n\
\n\
$graphtrace settime $maxtime 1\n\
\n\
set w $tlw_.graph\n\
if {[winfo exists $w]==0} {\n\
frame $w\n\
pack $w -side top -fill x -expand true -after [$vslider frame]\n\
}\n\
\n\
lappend graphs $netgraph\n\
frame $w.f$name -borderwidth 2 -relief groove\n\
pack $w.f$name -side top -expand true -fill both\n\
label $w.f$name.pr -bitmap pullright -borderwidth 1 -relief raised\n\
bind $w.f$name.pr <Enter> \\\n\
\"$self viewgraph_label \\\"node of no energy left\\\" \\\n\
$w.f$name $w.f$name.pr $netgraph\"\n\
pack $w.f$name.pr -side left\n\
$netgraph view $w.f$name.view\n\
\n\
$netgraph settime $now\n\
\n\
pack $w.f$name.view -side left -expand true \\\n\
-fill both\n\
frame $w.f$name.l2 -width [expr [$vslider swidth]/2] -height 30\n\
pack $w.f$name.l2 -side left\n\
$windows(title) configure -text $nam_name\n\
\n\
}\n\
\n\
\n\
Animator instproc view_drag_start {view x y} {\n\
$self instvar drag\n\
set drag($view,x) $x\n\
set drag($view,y) $y\n\
}\n\
\n\
Animator instproc view_drag_motion {view x y} {\n\
$self instvar drag\n\
set MAX_DRAG_LENGTH 0.008\n\
\n\
set dx [expr ($drag($view,x) - $x)]\n\
set dy [expr ($drag($view,y) - $y)]\n\
\n\
if {$dx > $MAX_DRAG_LENGTH} {\n\
set dx $MAX_DRAG_LENGTH\n\
}\n\
\n\
if {$dx < -$MAX_DRAG_LENGTH} {\n\
set dx -$MAX_DRAG_LENGTH\n\
}\n\
\n\
if {$dy > $MAX_DRAG_LENGTH} {\n\
set dy $MAX_DRAG_LENGTH\n\
}\n\
\n\
if {$dy < -$MAX_DRAG_LENGTH} {\n\
set dy -$MAX_DRAG_LENGTH\n\
}\n\
\n\
$view xview scroll $dx world\n\
$view yview scroll $dy world\n\
$self view_drag_start $view $x $y\n\
}\n\
\n\
Animator instproc new_editview {} {\n\
$self instvar netModel nam_name NETWORK_MODEL tlw_\n\
\n\
if [winfo exists $tlw_.editview] { \n\
return \n\
}\n\
toplevel $tlw_.editview\n\
set w $tlw_.editview\n\
\n\
wm title $w $nam_name\n\
frame $w.f\n\
frame $w.f.f -borderwidth 2 -relief sunken\n\
$netModel editview $w.f.f.edit\n\
set newView $w.f.f.edit\n\
pack $w.f.f.edit -side top -expand true -fill both\n\
\n\
$newView xscroll $w.f.hsb\n\
scrollbar $w.f.hsb -orient horizontal -width 10 -borderwidth 1 \\\n\
-command \"$newView xview\"\n\
$w.f.hsb set 0.0 1.0\n\
pack $w.f.hsb -side bottom -fill x\n\
pack $w.f.f -side top -fill both -expand true\n\
\n\
frame $w.f2\n\
$newView yscroll $w.f2.vsb\n\
scrollbar $w.f2.vsb -orient vertical -width 10 -borderwidth 1 \\\n\
-command \"$newView yview\"\n\
$w.f2.vsb set 0.0 1.0\n\
pack $w.f2.vsb -side top -fill y -expand true\n\
frame $w.f2.l -width 12 -height 12\n\
pack $w.f2.l -side top\n\
\n\
frame $w.ctrl -borderwidth 2 -relief groove\n\
$self build-zoombar $newView $w.ctrl $w\n\
pack $w.ctrl -side left -fill y\n\
\n\
pack $w.f2 -side right -fill y\n\
pack $w.f -side left -fill both -expand true\n\
\n\
$self editview_bind $w.f.f.edit\n\
}\n\
\n\
Animator instproc editview_bind { ev } {\n\
bind $ev <ButtonPress-1> \"$ev setPoint \\%x \\%y 0\"\n\
\n\
bind $ev <Shift-ButtonPress-1> \"$ev setPoint \\%x \\%y 1\"\n\
\n\
bind $ev <ButtonRelease-3> \"$ev dctag\"\n\
\n\
bind $ev <ButtonRelease-1> \"$ev releasePoint \\%x \\%y\"\n\
\n\
bind $ev <Any-B1-Motion> \"$ev moveTo \\%x \\%y\"\n\
}\n\
\n\
Animator instproc clear_editview_bind { ev } {\n\
bind $ev <ButtonPress-1> \"\"\n\
bind $ev <Shift-ButtonPress-1> \"\"\n\
bind $ev <ButtonRelease-3> \"\"\n\
bind $ev <ButtonRelease-1> \"\"\n\
bind $ev <Any-B1-Motion> \"\"\n\
$ev view_mode\n\
}\n\
\n\
set montab(l) graph\n\
set montab(n) node\n\
set montab(p) monitor\n\
set montab(d) monitor\n\
set montab(r) monitor\n\
set montab(a) monitor\n\
\n\
Animator instproc graph_info { netView x y } {\n\
$self instvar running resume tracefile\n\
$self tkvar nowDisp\n\
catch {destroy $netView.f}\n\
frame $netView.f -relief groove -borderwidth 2\n\
set name [$netView getname $nowDisp $x $y]\n\
set type [lindex $name 0]\n\
switch $type {\n\
\"l\" {\n\
set e1 [lindex $name 1]\n\
set e2 [lindex $name 2]\n\
frame $netView.f.f1 -borderwidth 2 -relief groove\n\
pack $netView.f.f1 -side left\n\
label $netView.f.f1.l -text \"Graph bandwidth\" -font [smallfont]\n\
pack $netView.f.f1.l -side top\n\
frame $netView.f.f1.f -borderwidth 0\n\
pack $netView.f.f1.f -side top\n\
button $netView.f.f1.f.b2 -relief raised -highlightthickness 0 \\\n\
-text \"Link $e1->$e2\" -font [smallfont]\\\n\
-command \"$self end_info $netView;\\\n\
$self viewgraph \\\"l $e1 $e2\\\" bw $tracefile\"\n\
pack $netView.f.f1.f.b2 -side left -fill x -expand true\n\
button $netView.f.f1.f.b3 -relief raised -highlightthickness 0 \\\n\
-text \"Link $e2->$e1\" -font [smallfont] \\\n\
-command \"$self end_info $netView;\\\n\
$self viewgraph \\\"l $e2 $e1\\\" bw $tracefile\"\n\
pack $netView.f.f1.f.b3 -side left -fill x -expand true\n\
\n\
frame $netView.f.f2 -borderwidth 2 -relief groove\n\
pack $netView.f.f2 -side left\n\
label $netView.f.f2.l -text \"Graph loss\" -font [smallfont]\n\
pack $netView.f.f2.l -side top\n\
frame $netView.f.f2.f -borderwidth 0\n\
pack $netView.f.f2.f -side top\n\
button $netView.f.f2.f.b2 -relief raised -highlightthickness 0 \\\n\
-text \"Link $e1->$e2\" -font [smallfont]\\\n\
-command \"$self end_info $netView;\\\n\
$self viewgraph \\\"l $e1 $e2\\\" loss $tracefile\"\n\
pack $netView.f.f2.f.b2 -side left -fill x -expand true\n\
button $netView.f.f2.f.b3 -relief raised -highlightthickness 0 \\\n\
-text \"Link $e2->$e1\" -font [smallfont] \\\n\
-command \"$self end_info $netView;\\\n\
$self viewgraph \\\"l $e2 $e1\\\" loss $tracefile\"\n\
pack $netView.f.f2.f.b3 -side left -fill x -expand true\n\
}\n\
}\n\
button $netView.f.d -relief raised -highlightthickness 0 \\\n\
-text \"Dismiss\" -font [smallfont]\\\n\
-command \"$self end_info $netView\"\n\
pack $netView.f.d -side left  -anchor sw\n\
\n\
place_frame $netView $netView.f $x $y\n\
}\n\
\n\
Animator instproc start_info { netView x y key_} {\n\
$self instvar running resume tracefile \n\
$self instvar fid pktType eSrc eDst\n\
$self tkvar nowDisp\n\
global montab\n\
\n\
if {($running) || ([winfo exists $netView.f] && ($resume == 1))} {\n\
set resume 1 \n\
} else {\n\
set resume 0\n\
}\n\
\n\
$self stop 1\n\
\n\
set text [string trim [$netView info $nowDisp $x $y] \"\\n\"]\n\
if { [string length $text] > 0 } {\n\
set name [$netView getname $nowDisp $x $y]\n\
set fid [$netView getfid $nowDisp $x $y]\n\
set pktType [$netView gettype $nowDisp $x $y]\n\
set eSrc [$netView getesrc $nowDisp $x $y]\n\
set eDst [$netView getedst $nowDisp $x $y]\n\
set type [lindex $name 0]\n\
\n\
if {$type==\"\"} {\n\
puts \"got an empty object name returned from object $text!\"\n\
return\n\
}\n\
catch {destroy $netView.f}\n\
\n\
frame $netView.f -relief groove -borderwidth 2\n\
\n\
message $netView.f.msg -width 8c -text $text \\\n\
-font [option get . smallfont Nam]\n\
pack $netView.f.msg -side top\n\
\n\
frame $netView.f.f\n\
pack $netView.f.f -side top -fill x -expand true\n\
\n\
if {$montab($type)==\"monitor\"} {\n\
button $netView.f.f.b -relief raised -highlightthickness 0 \\\n\
-text \"Monitor\" \\\n\
-command \"$self monitor $netView $nowDisp $x $y; \\\n\
$self end_info $netView\"\n\
pack $netView.f.f.b -side left -fill x -expand true\n\
button $netView.f.f.b1 -relief raised -highlightthickness 0 \\\n\
-text \"Filter\" \\\n\
-command \"$self filter_menu ; \\\n\
$self end_info $netView\"\n\
pack $netView.f.f.b1 -side left -fill x -expand true\n\
\n\
} elseif {$montab($type)==\"graph\"} {\n\
button $netView.f.f.b2 -relief raised -highlightthickness 0 \\\n\
-text \"Graph\" \\\n\
-command \"$self graph_info $netView $x $y\"\n\
pack $netView.f.f.b2 -side left -fill x -expand true\n\
\n\
} elseif {$montab($type)==\"node\"} {\n\
set node_id [lindex $name 1]\n\
button $netView.f.f.b3 -relief raised -highlightthickness 0 \\\n\
-text \"Filter\" \\\n\
-command \"$self node_filter_menu $node_id; \\\n\
$self end_info $netView\"\n\
pack $netView.f.f.b3 -side left -fill x -expand true\n\
\n\
set node_tclscript [$netView get_node_tclscript $node_id]\n\
if {$node_tclscript != \"\"} {\n\
set node_tclscript_label [$netView get_node_tclscript_label $node_id]\n\
button $netView.f.f.b4 -relief raised -highlightthickness 0 \\\n\
-text $node_tclscript_label \\\n\
-command $node_tclscript\n\
pack $netView.f.f.b4 -side left -fill x -expand true\n\
}\n\
\n\
}\n\
\n\
button $netView.f.f.d -relief raised -highlightthickness 0 \\\n\
-text \"Dismiss\" \\\n\
-command \"$self end_info $netView\"\n\
\n\
pack $netView.f.f.d -side left -fill x -expand true\n\
place_frame $netView $netView.f $x $y\n\
} else {\n\
$self end_info $netView\n\
}\n\
}\n\
\n\
Animator instproc node_filter_menu {nodeId} { \n\
$self tkvar setSrc_ setDst_ \n\
toplevel .nodeFilterMenu\n\
set w .nodeFilterMenu\n\
wm title $w \"Only show packet with this node as its \"\n\
checkbutton $w.c1  -text \"Source address\" \\\n\
-variable [$self tkvarname setSrc_] -anchor w\n\
checkbutton $w.c2  -text \"Destination address\" \\\n\
-variable [$self tkvarname setDst_] -anchor w\n\
button $w.b1  -relief raised -highlightthickness 0 \\\n\
-text \"Close\" -command \"$self set_node_filters $w $nodeId\"\n\
pack $w.c1 $w.c2 $w.b1 -side top -fill x -expand true\n\
}\n\
\n\
\n\
Animator instproc filter_menu {} { \n\
$self instvar colorarea colorn\n\
$self tkvar setFid_ setSrc_ setDst_ setTraffic_ filter_action\n\
toplevel .filterMenu\n\
set w .filterMenu\n\
wm title $w \"Only show packet with the same\"\n\
frame $w.f1 -borderwidth 2 -relief groove\n\
pack $w.f1 -side top -fill x\n\
checkbutton $w.f1.c1  -text \"Flow id\" \\\n\
-variable [$self tkvarname setFid_] -anchor w\n\
checkbutton $w.f1.c2  -text \"Source address\" \\\n\
-variable [$self tkvarname setSrc_] -anchor w\n\
checkbutton $w.f1.c3  -text \"Destination address\" \\\n\
-variable [$self tkvarname setDst_] -anchor w\n\
checkbutton $w.f1.c4  -text \"traffic type\" \\\n\
-variable [$self tkvarname setTraffic_] -anchor w\n\
pack $w.f1.c1 $w.f1.c2 $w.f1.c3 $w.f1.c4 -side top -fill x -expand true\n\
frame $w.f2 -borderwidth 2 -relief groove\n\
pack $w.f2 -side top -fill x\n\
frame $w.f2.a1 -borderwidth 2 -relief groove\n\
radiobutton $w.f2.a1.r  -text \"Color packet\" \\\n\
-variable [$self tkvarname filter_action] -value color -anchor w\n\
button $w.f2.a1.b  -relief raised -highlightthickness 0 \\\n\
-text \"Color\" -command \"$self selectColor\"\n\
set colorarea $w.f2.a1.c\n\
text $w.f2.a1.c -width 15 -height 1\n\
if {[info exists colorn] } { \n\
$colorarea insert 0.0 $colorn\n\
catch { $colorarea tag add bgcolor 0.0 end }\n\
$colorarea tag configure bgcolor -background $colorn\n\
} else {\n\
$colorarea insert 0.0 \"black\" \n\
}\n\
pack $w.f2.a1.r $w.f2.a1.b $w.f2.a1.c -side left -fill y -expand true\n\
radiobutton $w.f2.a2  -text \"Show only packet\" \\\n\
-variable [$self tkvarname filter_action] -value showpkt -anchor w\n\
radiobutton $w.f2.a3  -text \"Don't show packet\" \\\n\
-variable [$self tkvarname filter_action] -value hidepkt -anchor w\n\
pack $w.f2.a1 $w.f2.a2 $w.f2.a3  -side top -fill x -expand true\n\
button $w.b1  -relief raised -highlightthickness 0 \\\n\
-text \"Close\" -command \"$self set_filters $w\"\n\
pack $w.b1 -side top -fill x -expand true\n\
set filter_action color\n\
}\n\
\n\
Animator instproc resetAll {} { \n\
$self instvar netModel \n\
$self tkvar setFid_ setSrc_ setDst_ setTraffic_\n\
$self tkvar setFidC_ setSrcC_ setDstC_ setTrafficC_\n\
set setFid_ 0\n\
set setSrc_ 0\n\
set setDst_ 0\n\
set setTraffic_ 0\n\
set setFidC_ 0\n\
set setSrcC_ 0\n\
set setDstC_ 0\n\
set setTrafficC_ 0\n\
\n\
$netModel resetFilter\n\
\n\
}\n\
\n\
\n\
Animator instproc set_filters {w} { \n\
$self instvar fid netModel pktType eSrc eDst \n\
$self tkvar setFid_ setSrc_ setDst_ setTraffic_ filter_action\n\
if {$filter_action==\"showpkt\"} {\n\
if $setFid_ {\n\
$netModel fid_filter $fid\n\
} else {\n\
$netModel fid_filter -1\n\
}\n\
if $setTraffic_ {\n\
$netModel traffic_filter $pktType\n\
} else {\n\
$netModel traffic_filter \"\"\n\
}\n\
if $setSrc_ {\n\
$netModel src_filter $eSrc\n\
} else {\n\
$netModel src_filter -1\n\
}\n\
if $setDst_ {\n\
$netModel dst_filter $eDst\n\
} else {\n\
$netModel dst_filter -1\n\
} \n\
} elseif {$filter_action==\"color\"} {\n\
if $setFid_ {\n\
$netModel color-fid $fid\n\
} else {\n\
$netModel color-fid -1\n\
}\n\
if $setTraffic_ {\n\
$netModel color-traffic $pktType\n\
} else {\n\
$netModel color-traffic \"\"\n\
}\n\
if $setSrc_ {\n\
$netModel color-src $eSrc\n\
} else {\n\
$netModel color-src -1\n\
}\n\
if $setDst_ {\n\
$netModel color-dst $eDst\n\
} else {\n\
$netModel color-dst -1\n\
}\n\
} elseif {$filter_action==\"hidepkt\"} {\n\
if $setFid_ {\n\
$netModel hide-fid $fid\n\
} else {\n\
$netModel hide-fid -1\n\
}\n\
if $setTraffic_ {\n\
$netModel hide-traffic $pktType\n\
} else {\n\
$netModel hide-traffic \"\"\n\
}\n\
if $setSrc_ {\n\
$netModel hide-src $eSrc\n\
} else {\n\
$netModel hide-src -1\n\
}\n\
if $setDst_ {\n\
$netModel hide-dst $eDst\n\
} else {\n\
$netModel hide-dst -1\n\
}\n\
}\n\
destroy $w\n\
}\n\
\n\
\n\
Animator instproc set_node_filters {w nodeId} { \n\
$self instvar netModel \n\
$self tkvar setSrc_ setDst_ \n\
if $setSrc_ {\n\
$netModel src_filter $nodeId\n\
} else {\n\
$netModel src_filter -1\n\
}\n\
if $setDst_ {\n\
$netModel dst_filter $nodeId\n\
} else {\n\
$netModel dst_filter -1\n\
}\n\
destroy $w\n\
}\n\
\n\
\n\
Animator instproc end_info {netView} { \n\
$self instvar resume\n\
catch { destroy $netView.f }\n\
if $resume \"$self play 1\"\n\
}\n\
\n\
\n\
Animator instproc monitor {netView now x y} {\n\
$self instvar windows \n\
$self tkvar showpanel\n\
\n\
set mon [$netView new_monitor $now $x $y]\n\
set showpanel(monitor) 1\n\
if {$mon>=0} {\n\
$self create_monitor $netView $mon $now\n\
}\n\
}\n\
\n\
Animator instproc monitor_agent {node agent} {\n\
$self instvar netViews netModel \n\
$self tkvar nowDisp showpanel\n\
\n\
set netView [lindex $netViews 0]\n\
set mon [$netModel new_monitor_agent $node $agent]\n\
set showpanel(monitor) 1\n\
if {$mon>=0} {\n\
$self create_monitor $netView $mon $nowDisp\n\
}\n\
}\n\
\n\
Animator instproc create_monitor {netView mon now} {\n\
$self instvar windows monitors maxmon\n\
frame $windows(monitor).f$maxmon -borderwidth 2 -relief groove\n\
pack $windows(monitor).f$maxmon -side left -fill y\n\
set monitors($mon) $windows(monitor).f$maxmon\n\
set w $windows(monitor).f$maxmon\n\
message $w.l -aspect 300 -text \"\\[$mon\\] [$netView monitor $now $mon]\" \\\n\
-anchor nw -font [option get . smallfont Nam]\n\
pack $w.l -side top -anchor w -fill both -expand true\n\
bind $w.l <Enter> \\\n\
\"$w.l configure -background [option get . activeBackground Nam]\"\n\
bind $w.l <Leave> \\\n\
\"$w.l configure -background [option get . background Nam]\"\n\
bind $w.l <1> \"$self delete_monitor $netView $mon\"\n\
incr maxmon\n\
}\n\
\n\
Animator instproc update_monitors {netView now} {\n\
$self instvar monitors\n\
foreach mon [array names monitors] {\n\
set text [$netView monitor $now $mon]\n\
if {$text==\"\"} {\n\
$self delete_monitor $netView $mon\n\
} else {\n\
set text \"\\[$mon\\] $text\"\n\
if {[$monitors($mon).l configure -text]!=$text} {\n\
$monitors($mon).l configure -text \"$text\"\n\
}\n\
}\n\
}\n\
}\n\
\n\
Animator instproc delete_monitor {netView mon} {\n\
$self instvar monitors\n\
$netView delete_monitor $mon\n\
catch {destroy $monitors($mon)}\n\
unset monitors($mon)\n\
$self redrawFrame\n\
}\n\
\n\
set stackthese \"h1 h2 h3 h4 h5 h6 b i ul ol dl a pre\"\n\
set www_cancels(/h1) \"h1 h2 h3 h4 h5 h6 i b\"\n\
set www_cancels(/h2) \"h1 h2 h3 h4 h5 h6 i b\"\n\
set www_cancels(/h3) \"h1 h2 h3 h4 h5 h6 i b\"\n\
set www_cancels(/h4) \"h1 h2 h3 h4 h5 h6 i b\"\n\
set www_cancels(/h5) \"h1 h2 h3 h4 h5 h6 i b\"\n\
set www_cancels(/h6) \"h1 h2 h3 h4 h5 h6 i b\"\n\
set www_cancels(h1) \"h1 h2 h3 h4 h5 h6 i b\"\n\
set www_cancels(h2) \"h1 h2 h3 h4 h5 h6 i b\"\n\
set www_cancels(h3) \"h1 h2 h3 h4 h5 h6 i b\"\n\
set www_cancels(h4) \"h1 h2 h3 h4 h5 h6 i b\"\n\
set www_cancels(h5) \"h1 h2 h3 h4 h5 h6 i b\"\n\
set www_cancels(h6) \"h1 h2 h3 h4 h5 h6 i b\"\n\
set www_cancels(/b) \"b\"\n\
set www_cancels(/strong) \"strong\"\n\
set www_cancels(/i) \"i\"\n\
set www_cancels(/ul) \"ul\"\n\
set www_cancels(/ol) \"ol\"\n\
set www_cancels(/dl) \"dl\"\n\
set www_cancels(/a) \"a\"\n\
set www_cancels(/pre) \"pre\"\n\
set www_sets(h1) {{bold 1} {size 18}}\n\
set www_sets(h2) {{bold 1} {size 14}} ;# may not exists on some systems\n\
set www_sets(h3) {{bold 1} {size 14}}\n\
set www_sets(h4) {{bold 1} {size 12}}\n\
set www_sets(h5) {{bold 1} {size 10}}\n\
set www_sets(h6) {{bold 1} {size 10}}\n\
set www_sets(i) {{italic 1} {bold 0}}\n\
set www_sets(b) {{italic 0} {bold 1}}\n\
set www_sets(strong) {{italic 0} {bold 1}}\n\
set www_sets(verbatim) {{verbatim 1} {font courier}}\n\
set www_sets(normal) {{bold 0} {italic 0} {font helvetica} {size 12} {nl 0} {hr 0} {ltype \"\"} {bullet 0} {verbatim 0} {indent 0} {href \"\"}}\n\
set www_sets(ul) {{ltype ul} {indent [expr $indent+4]}}\n\
set www_sets(ol) {{ltype ol} {indent [expr $indent+4]}}\n\
set www_sets(dl) {{ltype dl} {indent [expr $indent+4]}}\n\
set www_sets(a) {{href XXX}}\n\
set www_runs(a) {extract_anchor}\n\
set www_runs(img) {extract_image}\n\
\n\
set www_fsets(p) {{nl 2}}\n\
set www_fsets(br) {{nl 1}}\n\
set www_fsets(hr) {{hr 1}}\n\
set www_fsets(h1) {{nl 2}}\n\
set www_fsets(h2) {{nl 2}}\n\
set www_fsets(h3) {{nl 2}}\n\
set www_fsets(h4) {{nl 2}}\n\
set www_fsets(h5) {{nl 2}}\n\
set www_fsets(h6) {{nl 2}}\n\
set www_fsets(ul) {{nl 0}}\n\
set www_fsets(ol) {{nl 0}}\n\
set www_fsets(dl) {{nl 0}}\n\
set www_fsets(/h1) {{nl 2}}\n\
set www_fsets(/h2) {{nl 2}}\n\
set www_fsets(/h3) {{nl 2}}\n\
set www_fsets(/h4) {{nl 2}}\n\
set www_fsets(/h5) {{nl 2}}\n\
set www_fsets(/h6) {{nl 2}}\n\
set www_fsets(/ul) {{nl 1}}\n\
set www_fsets(/ol) {{nl 1}}\n\
set www_fsets(/dl) {{nl 1}}\n\
set www_fsets(li) {{nl 1} {bullet 1}}\n\
set www_fsets(dt) {{nl 1}}\n\
\n\
set fonts(normal) -*-helvetica-medium-r-normal--12-*\n\
set fonts(helvetica,0,0,16) -*-helvetica-medium-r-normal--17-*\n\
set fonts(helvetica,1,0,16) -*-helvetica-bold-r-normal--17-*\n\
set fonts(helvetica,1,1,16) -*-helvetica-bold-o-normal--17-*\n\
set fonts(helvetica,1,0,18) -*-helvetica-bold-r-normal--18-*\n\
set fonts(helvetica,1,1,18) -*-helvetica-bold-o-normal--18-*\n\
set fonts(helvetica,1,0,14) -*-helvetica-bold-r-normal--14-*\n\
set fonts(helvetica,1,1,14) -*-helvetica-bold-o-normal--14-*\n\
set fonts(helvetica,1,0,12) -*-helvetica-bold-r-normal--12-*\n\
set fonts(helvetica,1,1,12) -*-helvetica-bold-o-normal--12-*\n\
set fonts(helvetica,0,1,12) -*-helvetica-medium-o-normal--12-*\n\
set fonts(helvetica,1,0,10) -*-helvetica-bold-r-normal--10-*\n\
\n\
AnimControl instproc parse_html {w entry text} {\n\
global www_sets www_fsets www_cancels www_runs stackthese fonts \n\
$self instvar href href_keep www_win\n\
set www_win $w\n\
set stack normal\n\
set size normal\n\
set font helvetica\n\
set bold 0\n\
set italic 0\n\
set underline 0\n\
set verbatim 0\n\
set indent 0\n\
set nl 0\n\
set hr 0\n\
set prevnl 0\n\
set href \"\"\n\
set bullet 0\n\
set ltype \"\"\n\
set parts [split $text \"<>\"]\n\
set istag 0\n\
$w delete 1.0 end\n\
foreach part $parts {\n\
if {$istag==1} {\n\
set tag [string tolower [lindex $part 0]]\n\
set c \"\"\n\
catch {set c $www_cancels($tag)}\n\
set rm 0\n\
set newstack {}\n\
foreach frame $stack {\n\
set newframe {}\n\
if {([lsearch -exact $c $frame]>=0)&&($rm==0)} {\n\
set rm 1\n\
} else {\n\
lappend newstack $frame\n\
}\n\
}\n\
set stack $newstack\n\
set newframe $tag\n\
\n\
foreach frame \"$stack $newframe\" {\n\
set t [lindex $frame 0]\n\
set s \"\"\n\
catch {set s $www_sets($t)}\n\
foreach item $s {\n\
set cmd \"set $item\"\n\
eval $cmd\n\
}\n\
}\n\
\n\
set s \"\"\n\
catch {set s $www_fsets($tag)}\n\
foreach item $s {\n\
set cmd \"set $item\"\n\
eval $cmd\n\
}\n\
\n\
set s \"\"\n\
catch {set s $www_runs($tag)}\n\
foreach item $s {\n\
set cmd \"$self $item $part\"\n\
eval $cmd\n\
}\n\
\n\
if {[lsearch -exact $stackthese $tag]>=0} {\n\
lappend stack $newframe\n\
}\n\
\n\
} else {\n\
if {$hr==1} {\n\
$w insert end \"\\n_____________________________________________________\\n\"\n\
if {$prevnl==0} {set prevnl 1}\n\
}\n\
\n\
for {set i 0} {$i < [expr $nl-$prevnl]} {incr i} {\n\
$w insert end \"\\n\"\n\
}\n\
set prevnl $nl\n\
\n\
if {$nl>0} {\n\
for {set i 0} {$i < $indent} {incr i} {\n\
$w insert end \" \"\n\
}\n\
}\n\
\n\
if {$bullet!=0} {\n\
$w insert end \"* \"\n\
}\n\
\n\
set ix [$w index end-1c]\n\
if {$nl>0} {\n\
set part [string trimleft $part]\n\
}\n\
if {$verbatim==0} {\n\
set part [remove_nl $part]\n\
if {[string length $part]>0} {\n\
set prevnl 0\n\
}\n\
$w insert end $part\n\
} else {\n\
$w insert end $part\n\
}\n\
\n\
$self instvar tagnum\n\
if {($size!=12)||($bold==1)||($italic==1)||($href!=\"\")||\\\n\
($font!=\"helvetica\")} {\n\
incr tagnum\n\
set ix2 [$w index end-1c]\n\
$w tag add t$tagnum $ix end-1c\n\
}\n\
if {($size!=12)||($bold==1)||($italic==1)||\\\n\
($font!=\"helvetica\")} {\n\
set thisfont $fonts(normal)\n\
catch {\n\
set thisfont $fonts($font,$bold,$italic,$size)\n\
}\n\
$w tag configure t$tagnum -font $thisfont\n\
}\n\
if {$href!=\"\"} {\n\
$w tag configure t$tagnum -foreground red\n\
$w tag bind t$tagnum <Enter> \\\n\
\"$self set_entry $entry $href_keep\"\n\
$w tag bind t$tagnum <Leave> \\\n\
\"$self set_entry $entry {}\"\n\
$w tag bind t$tagnum <1> \"$self goto $href_keep $w $entry\"\n\
}\n\
\n\
}\n\
set istag [expr 1-$istag]\n\
}\n\
}\n\
\n\
AnimControl instproc set_entry {entry str} {\n\
$entry delete 0 end\n\
$entry insert 0 $str\n\
}\n\
\n\
AnimControl instproc extract_anchor args {\n\
$self instvar href_keep href\n\
set href_keep \"\"\n\
set href \"\"\n\
foreach arg $args {\n\
if {[string range [string tolower $arg] 0 4]==\"href=\"} {\n\
set href_keep [string trim [string range $arg 5 end] {\"}]\n\
set href XXX\n\
}	    \n\
}\n\
}\n\
\n\
AnimControl instproc extract_image args {\n\
$self instvar www_win wix\n\
set img \"\"\n\
foreach arg $args {\n\
if {[string range [string tolower $arg] 0 3]==\"src=\"} {\n\
set img [string trim [string range $arg 4 end] {\"}]\n\
}	    \n\
}\n\
$www_win window create end -create \"label $www_win.w$wix -bitmap $img\"\n\
incr wix\n\
}\n\
\n\
AnimControl instproc goto {url win entry} {\n\
global help_text\n\
set res \"xxxx\"\n\
set scheme [lindex [split $url \":\"] 0]\n\
if {$scheme==\"help\"} {\n\
$self parse_html $win $entry $help_text([string range $url 5 end])\n\
return\n\
}\n\
set proggy netscape\n\
catch {\n\
set res [exec $proggy -display :0 -remote openURL($url)]\n\
}\n\
if {$res==\"xxxx\"} {\n\
catch {\n\
set res [exec $proggy -display :0.1 -remote openURL($url)]\n\
}\n\
}\n\
if {$res==\"xxxx\"} {\n\
puts \"failed to pass URL to $proggy\"\n\
}\n\
}\n\
\n\
proc lreverse {list} {\n\
set res {}\n\
foreach el $list {\n\
set res [concat [list $el] $res]\n\
}\n\
return $res\n\
}\n\
\n\
proc remove_nl {str} {\n\
set parts [split $str \"\\n\\r\"] \n\
set res \"\"\n\
set space 1\n\
for {set i 0} {$i<[llength $parts]} {incr i} {\n\
set part [lindex $parts $i]\n\
if {$i>0} {\n\
set part [string trimleft $part]\n\
} else {\n\
if {$part==\"\"} {set space 0}\n\
}\n\
if {($i+1)<[llength $parts]} {\n\
set part [string trimright $part]\n\
}\n\
if {$space==0} {\n\
set res \"$res \"\n\
set space 1\n\
} \n\
if {[string length $part] > 0} {\n\
set res \"$res$part\"\n\
set space 0\n\
}\n\
}\n\
return $res\n\
}\n\
\n\
set help_text(home) \"<h1>NAM - The Network Animator</h1>\n\
Welcome to Nam 1.15\n\
<P>\n\
Developed by UCB and the VINT, SAMAN, and Conser projects at ISI.\n\
<P>\n\
Nam contains source code with the following copyrights:\n\
<P>\n\
Copyright (c) 1991-1994 Regents of the University of California.\n\
<BR>\n\
Copyright (c) 1997-1999 University of Southern California\n\
<BR>\n\
Copyright (c) 2000-2002 USC/Information Sciences Institute\n\
<P>\n\
\"\n\
\n\
\n\
set help_text(about) \"<h1>NAM - The Network Animator</h1>\n\
This version of nam is highly experimental - there will be bugs!.  \n\
Please mail <I><a href=mailto:ns-users@isi.edu>ns-users@isi.edu</a></I> if you \n\
encounter any bugs, or with suggestions for desired functionality.\n\
<H2>History</h2>\n\
The network animator ``nam''\n\
began in 1990 as a simple tool for animating packet trace data.\n\
This trace data is typically\n\
derived as output from a network simulator like\n\
<a href=http://www-mash.cs.berkeley.edu/ns/>ns</a>\n\
or from real network measurements, e.g., using\n\
<a href=ftp://ftp.ee.lbl.gov/tcpdump.tar.Z>tcpdump</a>.\n\
<a href=http://www.cs.berkeley.edu/~mccanne>Steven McCanne</a>\n\
wrote the original version as a member of the\n\
<a href=http://www-nrg.ee.lbl.gov/>Network Research Group</a> at the\n\
<a href=http://www.lbl.gov/>Lawrence Berkeley National Laboratory</a>,\n\
and occasionally improved the design as he needed it in\n\
his research.  Marylou Orayani improved it further and used it\n\
for her Master's research over summer 1995 and into spring 1996.\n\
The nam development effort is now an ongoing\n\
collaboration at ISI in the SAMAN and Conser projects.\n\
Nam developers include:\n\
<a href=http://www.isi.edu/nsnam/>SAMAN and Conser projects</a>.\n\
<a href=http://netweb.usc.edu/vint/>VINT project</a>.\n\
<a href=http://north.east.isi.edu/~mjh/>Mark Handley</a>, \n\
<a href=http://www.isi.edu/~haoboy/>Haobo Yu</a>, <a href=http://www.isi.edu/~johnh/>John Heidemann</a>,\n\
<a href=http://www.isi.edu/~yaxu/>Ya Xu</a>, <a href=http://www.isi.edu/~kclan/>Kun-chan Lan</a>, and <a href=http://www-scf.usc.edu/~hyunahpa/>Hyunah Park</a>.\n\
<P>\n\
For information about ns and nam, please see\n\
<i><a href=http://mash.cs.berkeley.edu/nam/>http://mash.cs.berkeley.edu/nam/</a></i>.\n\
<h2>Funding</h2>\n\
Nam is currently funded by DARPA through the VINT project at LBL under\n\
DARPA grant DABT63-96-C-0105, at USC/ISI under DARPA grant\n\
ABT63-96-C-0054, at Xerox PARC under DARPA grant DABT63-96-C-0105. \n\
<H2>Copyright</H2>\n\
Nam contains source code with the following copyrights:\n\
<p>\n\
Copyright (c) 1991-1994 Regents of the University of California.\n\
<br>\n\
Copyright (c) 1997-1999 University of Southern California\n\
<P>\n\
Copyright (c) 2000-2001 USC/Information Sciences Institute\n\
<hr>\n\
<i><a href=help:help>Help Index</a><I>\n\
\"\n\
\n\
set help_text(help) \"\n\
<H1>Nam Quick Help</H1>\n\
<B>Use the controls to move the animation through time:</B>\n\
<DL>\n\
<DT><img src=rew>Rewind the animation by 25 times the current time step\n\
<DT><img src=back>Play the animation backwards\n\
<DT><img src=stop>Stop the playing of the animation\n\
<DT><img src=play>Play the animation normally\n\
<DT><img src=ff>Fast forward the animation by 25 times the current time step\n\
</DL>\n\
The current time step is selected by the time slider.\n\
<P>\n\
When the animation is running\n\
<UL><LI>Click the left button on an object in the display window to view information about that object.\n\
<LI>Select \\\"monitor\\\" from the resulting popup window to <a href=help:monitors>monitor that object</a> over time.\n\
<LI>The \\\"step\\\" slider can be used to control how fast time flows in the animation.\n\
The \\\"time\\\" slider can be used to move to a specific point in time.\n\
</UL>\n\
<P>\n\
<hr>\n\
<i><a href=help:about>About Nam</a><I>\n\
\"\n\
\n\
set help_text(monitors) \"\n\
<H1>Nam Help: Monitors</H1>\n\
You can set monitors on various animation objects to examine \n\
information about them over time.\n\
<H2>Packet Monitors</H2>\n\
To initiate a packet monitor, click the left button on the packet \n\
you wish to examine.  Information that nam has about that packet \n\
will then be displayed, and you are given the option to set a \n\
monitor on the packet.\n\
<P>\n\
Setting a monitor on a packet labels the packet so that you can \n\
more easily watch its flow through the network and through queues.\n\
An associated panel appears in the monitors window listing information \n\
about the packet if the packet is visible.  \n\
<P>\n\
Click on the monitor panel for a packet to remove that monitor.\n\
<H2>Agent Monitors</H2>\n\
Protocol agents are displayed alongside the node they are \n\
instantiated in.  To monitor an agent, click on it in the \n\
network display, and select \\\"monitor\\\".\n\
Information about the agent will be displayed in the monitors \n\
panel, and will be updated as protocol state in the agent \n\
changes over time.\n\
<P>\n\
Click on the monitor panel for the agent to remove that monitor.\n\
<hr>\n\
<I><a href=help:help>Return to help index</a>\n\
\"\n\
\n\
AnimControl instproc new_web {which} {\n\
global help_text\n\
if {[winfo exists .help]} {\n\
$self parse_html .help.text .help.f.ctl.e $help_text($which)\n\
return\n\
}\n\
toplevel .help\n\
wm title  .help \"About nam\"\n\
frame .help.f -relief groove -borderwidth 2\n\
pack  .help.f -side top -fill both -expand true\n\
frame .help.f.f\n\
pack  .help.f.f -side top -fill both -expand true\n\
text .help.f.f.w -width 60 -height 40 -wrap word -yscroll \".help.f.f.sb set\"\n\
bind .help.f.f.w <1> break\n\
pack .help.f.f.w -side left -fill both -expand true\n\
scrollbar .help.f.f.sb -command \".help.f.f.w yview\"\n\
pack .help.f.f.sb -side right -fill y\n\
frame .help.f.ctl -borderwidth 2 -relief groove\n\
pack .help.f.ctl -side top -fill x\n\
label .help.f.ctl.l -text \"go to:\"\n\
pack .help.f.ctl.l -side left\n\
\n\
entry .help.f.ctl.e -width 20 -borderwidth 1 -relief sunken\n\
pack .help.f.ctl.e -side left -fill x -expand true\n\
button .help.f.ctl.b -borderwidth 1 -relief raised -text Dismiss \\\n\
-command {destroy .help}\n\
pack .help.f.ctl.b -side left\n\
$self parse_html .help.f.f.w .help.f.ctl.e $help_text($which)\n\
}\n\
\n\
AnimControl instproc new_webhome {} {\n\
global help_text\n\
if {[winfo exists .help]} {\n\
$self parse_html .help.f.f.w .help.f.ctl.e $help_text(hich)\n\
return      \n\
}           \n\
\n\
set t [$self ScrolledText .help 60 15]\n\
\n\
frame .help.f -relief groove -borderwidth 2\n\
frame .help.f.ctl -borderwidth 2 -relief groove\n\
entry .help.f.ctl.e -width 20 -borderwidth 1 -relief sunken\n\
pack .help.f.ctl.e -side left -fill x -expand true\n\
button .help.f.ctl.b -borderwidth 1 -relief raised -text Dismiss \\\n\
-command {destroy .help}\n\
pack .help.f.ctl.b -side left\n\
$self parse_html $t .help.f.ctl.e $help_text(home)\n\
}\n\
\n\
AnimControl instproc ScrolledText { f width height } {\n\
frame $f\n\
text $f.text -width $width -height $height \\\n\
-setgrid true -wrap word \\\n\
-xscrollcommand [list $f.xscroll set] \\\n\
-yscrollcommand [list $f.yscroll set]\n\
scrollbar $f.xscroll -orient horizontal \\\n\
-command [list $f.text xview]\n\
scrollbar $f.yscroll -orient vertical \\\n\
-command [list $f.text yview]\n\
pack $f.xscroll -side bottom -fill x\n\
pack $f.yscroll -side right -fill y\n\
\n\
pack $f.text -side left -fill both -expand true\n\
pack $f -side top -fill both -expand true\n\
return $f.text\n\
}\n\
\n\
\n\
\n\
Animator instproc build-ui {} {\n\
$self instvar netView id_ tlw_ balloon_\n\
\n\
set tlw_ .model$id_\n\
toplevel $tlw_\n\
\n\
set balloon_ [new BalloonHelp $tlw_]\n\
\n\
frame $tlw_.menu -borderwidth 0\n\
$self build-menus $tlw_.menu\n\
pack $tlw_.menu -side top -fill x\n\
\n\
frame $tlw_.view\n\
$self build-view $tlw_.view\n\
\n\
frame $tlw_.ctrl0 -relief flat -borderwidth 0\n\
$self build-animation-ctrl $tlw_.ctrl0 \n\
pack $tlw_.ctrl0 -side top -fill x\n\
\n\
frame $tlw_.ctrl -relief flat -borderwidth 0\n\
$self build-view-ctrl $tlw_.ctrl\n\
set windows(control) $tlw_.ctrl\n\
pack $tlw_.ctrl -side bottom -fill x\n\
\n\
pack $tlw_.view -side top -fill both -expand true\n\
\n\
wm minsize $tlw_ 200 200\n\
$self window_bind $tlw_\n\
$self view_bind $netView\n\
\n\
bind $netView <Destroy> \"$self stop 1\"\n\
\n\
$self instvar tracefile\n\
wm title $tlw_ \"[tk appname]: $tracefile\"\n\
}\n\
\n\
Animator instproc build-view w {\n\
$self tkvar showpanel\n\
$self instvar netModel netView netViews tlw_ windows\n\
\n\
frame $w.f\n\
frame $w.f.f -borderwidth 2 -relief sunken\n\
$netModel editview $w.f.f.net\n\
set netView $w.f.f.net\n\
set netViews $netView\n\
pack $w.f.f.net -side top -expand true -fill both\n\
\n\
$netView xscroll $w.f.hsb\n\
scrollbar $w.f.hsb -orient horizontal -width 10 \\\n\
-borderwidth 1 -command \"$netView xview\"\n\
$w.f.hsb set 0.0 1.0\n\
pack $w.f.hsb -side bottom -fill x\n\
pack $w.f.f -side top -fill both -expand true\n\
\n\
frame $w.f2\n\
$netView yscroll $w.f2.vsb\n\
scrollbar $w.f2.vsb -orient vertical -width 10 \\\n\
-borderwidth 1 -command \"$netView yview\"\n\
$w.f2.vsb set 0.0 1.0\n\
pack $w.f2.vsb -side top -fill y -expand true\n\
frame $w.f2.l -width 12 -height 12\n\
pack $w.f2.l -side top\n\
\n\
frame $w.ctrl -borderwidth 2 -relief groove\n\
$self build-zoombar $netView $w.ctrl $w\n\
pack $w.ctrl -side left -fill y\n\
pack $w.f2 -side right -fill y\n\
pack $w.f -side left -fill both -expand true\n\
\n\
frame $tlw_.monwin -relief groove -borderwidth 2\n\
button $tlw_.monwin.l -bitmap monitors -borderwidth 2 -relief groove \\\n\
-command \"$self closepanel monitor\"\n\
pack $tlw_.monwin.l -side left -fill y\n\
set windows(monitor) $tlw_.monwin\n\
trace variable showpanel(monitor) w \\\n\
\"$self displaypanel $tlw_.monwin $w monitor top x false\"\n\
set showpanel(monitor) 0\n\
}\n\
\n\
Animator instproc build-view-ctrl {w} {\n\
$self tkvar showpanel\n\
$self instvar NETWORK_MODEL mintime maxtime now mslider pipemode \\\n\
vslider\n\
\n\
frame $w.p0b -relief flat -borderwidth 0\n\
\n\
\n\
set mslider [new TimesliderModel $mintime $maxtime $now $self]\n\
set vslider [new TimesliderView $w.p0b $mslider]\n\
\n\
$mslider addObserver $vslider\n\
$mslider setpipemode $pipemode\n\
\n\
frame $w.tl -relief flat -borderwidth 0\n\
$self build-annotation $w.tl\n\
set showpanel(annotate) 1\n\
\n\
if {$NETWORK_MODEL == \"NetworkModel/Auto\"} {\n\
set showpanel(autolayout) 1\n\
trace variable showpanel(autolayout) w \\\n\
\"$self displaypanel $w.p2 $w.p0b autolayout \\\n\
top x false\"\n\
frame $w.p2 -relief flat -borderwidth 0\n\
$self build-layout $w.p2\n\
pack $w.p0b $w.p2 $w.tl  \\\n\
-side top -fill x\n\
} else {\n\
set showpanel(autolayout) 0\n\
pack $w.p0b $w.tl -side top -fill x\n\
}\n\
}\n\
\n\
Animator instproc build-menus w {\n\
$self tkvar nam_record_animation nam_auto_ff showpanel isSync_\n\
$self tkvar showData_ showRouting_ showMac_\n\
$self instvar NETWORK_MODEL netModel nam_name windows \n\
\n\
frame $w.menu -relief groove -bd 2\n\
pack $w.menu -side top -fill x\n\
\n\
set padx 4\n\
\n\
set mb $w.menu.file\n\
set m $mb.m\n\
menubutton $mb -text \"File\" -menu $m -underline 0 \\\n\
-borderwidth 1 \n\
menu $m\n\
$m add command -label \"Save layout\" -command \"$self save_layout\"\n\
$m add command -label \"Print\" -command \"$self print_view $netModel nam\"\n\
$m add separator\n\
$m add checkbutton -label \"Record animation\" \\\n\
-variable [$self tkvarname nam_record_animation]\n\
if ![info exists nam_record_animation] {\n\
set nam_record_animation 0\n\
}\n\
$m add checkbutton -label \"Auto FastForward\" \\\n\
-variable [$self tkvarname nam_auto_ff]\n\
if ![info exists nam_auto_ff] {\n\
set nam_auto_ff 0\n\
}\n\
\n\
$m add separator\n\
$m add command -label \"Close\" -command \"$self done\"\n\
pack $mb -side left -padx $padx\n\
\n\
set mb $w.menu.views\n\
set m $mb.m\n\
set mp $m.filter\n\
set mp1 $mp.type1\n\
set mp2 $mp.type2\n\
menubutton $mb -text \"Views\" -menu $m -underline 0 \\\n\
-borderwidth 1 \n\
menu $m\n\
$m add command -label \"New view\" -command \"$self new_view\"\n\
$m add separator\n\
\n\
$m add command -label \"Node energy\" -command \"$self energy_view\"\n\
\n\
$m add cascade -label \"Packet filter\" -menu $mp\n\
menu $mp\n\
\n\
$mp add cascade -label \"Packet type\" -menu $mp1\n\
$mp add command -label \"Traffic type\" -command \"$self select-traffic\"\n\
$mp add command -label \"Source node\" -command \"$self select-src\"\n\
$mp add command -label \"Destination node\" -command \"$self select-dst\"\n\
$mp add command -label \"Flow id\" -command \"$self select-fid\"\n\
$mp add command -label \"Reset all\" -command \"$netModel resetFilter\"\n\
menu $mp1\n\
set showData_ 1\n\
set showRouting_ 1\n\
set showMac_ 1\n\
$mp1 add checkbutton -label \"Data\" \\\n\
-variable [$self tkvarname showData_]\n\
$mp1 add checkbutton -label \"Routing\" \\\n\
-variable [$self tkvarname showRouting_]\n\
$mp1 add checkbutton -label \"Mac\" \\\n\
-variable [$self tkvarname showMac_]\n\
\n\
\n\
$m add command -label \"Highlight leaf trees\" -command \\\n\
\"$self show_subtrees\"\n\
$m add checkbutton -label \"Show monitors\" \\\n\
-variable [$self tkvarname showpanel(monitor)]\n\
if {$NETWORK_MODEL==\"NetworkModel/Auto\"} {\n\
$m add checkbutton -label \"Show autolayout\" \\\n\
-variable [$self tkvarname showpanel(autolayout)]\n\
} else {\n\
$m add checkbutton -label \"Show autolayout\" -state disabled\n\
}\n\
$m add checkbutton -label \"Show annotations\" \\\n\
-variable [$self tkvarname showpanel(annotate)]\n\
$m add checkbutton -label \"Sync\" \\\n\
-variable [$self tkvarname isSync_]\n\
if ![info exists isSync_] {\n\
set isSync_ 0\n\
}\n\
trace variable isSync_ w \"$self on-change-sync\"\n\
pack $mb -side left -padx $padx\n\
\n\
set mb $w.menu.analysis\n\
set m $mb.m\n\
$self instvar analysis_OK\n\
if { $analysis_OK == 0 } {\n\
menubutton $mb -text \"Analysis\" -menu $m -underline 0 \\\n\
-borderwidth 1 -state disabled\n\
} else {\n\
menubutton $mb -text \"Analysis\" -menu $m -underline 0 \\\n\
-borderwidth 1\n\
}\n\
\n\
menu $m\n\
$m add command -label \"Active Sessions\" -command \"$self active_sessions\"\n\
$m add command -label \"Legend ...\" -command \"$self auto_legend\"\n\
pack $mb -side left -padx $padx\n\
\n\
label $w.menu.name -text $nam_name -font [smallfont] \\\n\
-width 30 -borderwidth 2 -relief groove \n\
set windows(title) $w.menu.name\n\
pack $w.menu.name -side left -fill x -expand true -padx 4 -pady 1\n\
\n\
}\n\
\n\
Animator instproc select-traffic {} {\n\
$self tkvar trafficType_\n\
toplevel .selectTraffic\n\
set w .selectTraffic\n\
wm title $w \"Enter traffic type (CBR,TCP,...)\"\n\
label $w.label -text \"Traffic type:\"\n\
entry $w.entry -width 50 -relief sunken \\\n\
-textvariable [$self tkvarname trafficType_]\n\
pack $w.label $w.entry -side left -fill both -expand true\n\
bind $w.entry <Return> \"$self filter-traffic $w\"\n\
}\n\
\n\
Animator instproc filter-traffic {w} {\n\
$self tkvar trafficType_\n\
$self instvar netModel \n\
destroy $w\n\
$netModel traffic_filter $trafficType_\n\
}\n\
\n\
Animator instproc select-src {} {\n\
$self tkvar srcNode_\n\
toplevel .selectSrc\n\
set w .selectSrc\n\
wm title $w \"Enter source node\"\n\
label $w.label -text \"Source node:\"\n\
entry $w.entry -width 50 -relief sunken \\\n\
-textvariable [$self tkvarname srcNode_]\n\
pack $w.label $w.entry -side left -fill both -expand true\n\
bind $w.entry <Return> \"$self filter-src $w\"\n\
}\n\
\n\
Animator instproc filter-src {w} {\n\
$self tkvar srcNode_\n\
$self instvar netModel \n\
destroy $w\n\
$netModel src_filter $srcNode_\n\
}\n\
\n\
Animator instproc select-dst {} {\n\
$self tkvar dstNode_\n\
toplevel .selectDst\n\
set w .selectDst\n\
wm title $w \"Enter destination node\"\n\
label $w.label -text \"Destination node:\"\n\
entry $w.entry -width 50 -relief sunken \\\n\
-textvariable [$self tkvarname dstNode_]\n\
pack $w.label $w.entry -side left -fill both -expand true\n\
bind $w.entry <Return> \"$self filter-dst $w\"\n\
}\n\
\n\
Animator instproc filter-dst {w} {\n\
$self tkvar dstNode_\n\
$self instvar netModel \n\
destroy $w\n\
$netModel dst_filter $dstNode_\n\
}\n\
\n\
Animator instproc select-fid {} {\n\
$self tkvar flowId_\n\
toplevel .selectFlow\n\
set w .selectFlow\n\
wm title $w \"Enter flow id\"\n\
label $w.label -text \"Flow id:\"\n\
entry $w.entry -width 50 -relief sunken \\\n\
-textvariable [$self tkvarname flowId_]\n\
pack $w.label $w.entry -side left -fill both -expand true\n\
bind $w.entry <Return> \"$self filter-fid $w\"\n\
}\n\
\n\
Animator instproc filter-fid {w} {\n\
$self tkvar flowId_\n\
$self instvar netModel \n\
destroy $w\n\
$netModel fid_filter $flowId_\n\
}\n\
\n\
\n\
Animator instproc selectColor {} {\n\
$self colorPaletteReq .colorpalette \\\n\
{0000 3300 6600 9900 CC00 FF00} \\\n\
{0000 3300 6600 9900 CC00 FF00} \\\n\
{0000 3300 6600 9900 CC00 FF00} \\\n\
.colorsp\n\
}\n\
\n\
Animator instproc colorPaletteReq { name redlist greenlist bluelist replace } {\n\
$self instvar SharedEnv netModel\n\
\n\
set w ${name}\n\
if {[winfo exists $w]} {\n\
wm deiconify $w\n\
raise $w\n\
return\n\
}\n\
set SharedEnv($name) $replace\n\
eval toplevel $w \"\"\n\
wm protocol $w WM_DELETE_WINDOW \"wm withdraw $w\"\n\
\n\
frame $w.f\n\
\n\
foreach red $redlist {\n\
frame $w.f.rcol_${red}\n\
foreach green $greenlist {\n\
frame $w.f.grow_${red}${green}\n\
foreach blue $bluelist {\n\
if { [info exists SharedEnv($w.f.c${red}${green}${blue})] } {\n\
frame $w.f.c${red}${green}${blue} \\\n\
-relief raised -height 2m -width 2m \\\n\
-highlightthickness 0 \\\n\
-bd 1 -bg $SharedEnv($w.f.c${red }${green}${blue})\n\
} else {\n\
frame $w.f.c${red}${green}${blue} \\\n\
-relief raised -height 2m -width 2m \\\n\
-highlightthickness 0 \\\n\
-bd 1 -bg \"#${red}${green}${blue}\"\n\
}\n\
pack $w.f.c${red}${green}${blue} -side left \\\n\
-in $w.f.grow_${red}${green} -fill both -expand true\n\
bind $w.f.c${red}${green}${blue} <ButtonRelease-1> \"\n\
$self setcolor %W\n\
\"\n\
}\n\
pack $w.f.grow_${red}${green} -side top \\\n\
-in $w.f.rcol_${red} -fill both -expand true\n\
}\n\
pack $w.f.rcol_${red} -side left -in $w.f -fill both \\\n\
-expand true\n\
}\n\
frame $w.f.c_none -width 4m -relief raised -bd 1 \\\n\
-highlightthickness 0\n\
pack $w.f.c_none -in $w.f -side left -fill y\n\
pack $w.f -in $w -expand true -fill both\n\
\n\
wm geometry $w 400x100\n\
}\n\
\n\
Animator instproc setcolor {w} {\n\
$self instvar netModel colorarea colorn\n\
\n\
$w configure -relief raised\n\
set colorv [$w cget -bg]\n\
set rgb [winfo rgb $w $colorv]\n\
set colorn \"[$netModel lookupColorName [lindex $rgb 0] \\\n\
[lindex $rgb 1] [lindex $rgb 2]]\"\n\
\n\
set rgb [winfo rgb . $colorn]\n\
set name \"[$netModel lookupColorName [lindex $rgb 0] \\\n\
[lindex $rgb 1] [lindex $rgb 2]]\"\n\
$netModel select-color $name\n\
\n\
if {[info exists colorarea] && [winfo exists $colorarea]} {\n\
$colorarea delete 0.0 end\n\
$colorarea insert 0.0 $colorn\n\
catch { $colorarea tag add bgcolor 0.0 end }\n\
$colorarea tag configure bgcolor -background $colorn\n\
}\n\
}\n\
\n\
Animator instproc on-change-sync args {\n\
$self tkvar isSync_\n\
if $isSync_ {\n\
$self add-sync $self\n\
} else {\n\
$self remove-sync $self\n\
}\n\
}\n\
\n\
Animator instproc set_run { w } {\n\
$self instvar netView running\n\
focus $netView\n\
$self set_forward_dir 1\n\
if { $running != 1 } {\n\
$self play 1\n\
}\n\
$self highlight $w.bar.run 1\n\
}\n\
\n\
Animator instproc set_back { w } {\n\
$self instvar netView running\n\
focus $netView\n\
$self set_backward_dir 1\n\
if { $running != 1 } {\n\
$self play 1\n\
$self renderFrame\n\
}\n\
$self highlight $w.bar.back 1\n\
}\n\
\n\
Animator instproc on-rateslider-press {} {\n\
$self instvar currRate prevRate\n\
set prevRate $currRate\n\
}\n\
\n\
Animator instproc on-rateslider-motion { v } {\n\
$self instvar timeStep stepDisp\n\
set timeStep [expr pow(10, $v / 10.)]\n\
set stepDisp [step_format $timeStep]\n\
}\n\
\n\
Animator instproc build-animation-ctrl {w} {\n\
$self instvar rateSlider granularity timeStep currRate prevRate \\\n\
stepDisp running direction balloon_\n\
\n\
set f [smallfont]\n\
frame $w.bar -relief groove -borderwidth 2\n\
frame $w.bar.rate -borderwidth 1 -relief sunken\n\
scale $w.bar.rate.s -orient horizontal -width 7p \\\n\
-label \"Step:\" -font [smallfont]\\\n\
-from -60 -to -1 -showvalue false \\\n\
-relief flat\\\n\
-borderwidth 1 -highlightthickness 0 \\\n\
-troughcolor [option get . background Nam]\n\
pack $w.bar.rate.s -side top -fill both -expand true\n\
set rateSlider $w.bar.rate.s\n\
set granularity [option get . granularity Nam]\n\
set timeStep [time2real [option get . rate Nam]]\n\
set stepDisp [step_format $timeStep]\n\
set currRate [expr 10*log10($timeStep)]\n\
set prevRate $currRate\n\
$rateSlider set $currRate\n\
bind $rateSlider <ButtonRelease-1> \"$self set_rate \\[%W get\\] 1\"\n\
bind $rateSlider <ButtonPress-1> \"$self on-rateslider-press\"\n\
bind $rateSlider <B1-Motion> \"$self on-rateslider-motion \\[%W get\\]\"\n\
trace variable stepDisp w \"$self displayStep\"\n\
\n\
$self tkvar nowDisp\n\
label $w.bar.timerVal -textvariable [$self tkvarname nowDisp] \\\n\
-width 14 -anchor w -font $f -borderwidth 1 \\\n\
-relief sunken -anchor e\n\
\n\
frame $w.bar.run\n\
button $w.bar.run.b -bitmap play -borderwidth 1 -relief raised \\\n\
-highlightthickness 0 -font $f  \\\n\
-command \"$self set_run $w\"\n\
frame $w.bar.run.f -height 5 -relief sunken \\\n\
-borderwidth 1\n\
pack $w.bar.run.b -side top -fill both -expand true\n\
pack $w.bar.run.f -side top -fill x\n\
$balloon_ balloon_for $w.bar.run.b \"play forward\" 1000\n\
\n\
frame $w.bar.back\n\
button $w.bar.back.b -bitmap back -borderwidth 1 -relief raised \\\n\
-highlightthickness 0 -font $f \\\n\
-command \"$self set_back $w\"\n\
$balloon_ balloon_for $w.bar.back.b \"play backward\" 1000\n\
\n\
trace variable running w \"$self trace_running_handler $w\"\n\
\n\
frame $w.bar.back.f -height 5 -relief sunken \\\n\
-borderwidth 1\n\
pack $w.bar.back.b -side top -fill both -expand true\n\
pack $w.bar.back.f -side top -fill x\n\
\n\
frame $w.bar.stop\n\
button $w.bar.stop.b -bitmap stop -borderwidth 1 -relief raised \\\n\
-highlightthickness 0 -font $f \\\n\
-command \"$self stop 1;\\\n\
$self renderFrame;\\\n\
$self highlight $w.bar.stop 1\"\n\
$balloon_ balloon_for $w.bar.stop.b \"stop\" 1000\n\
frame $w.bar.stop.f -height 5 -relief sunken \\\n\
-borderwidth 1\n\
pack $w.bar.stop.b -side top -fill both -expand true\n\
pack $w.bar.stop.f -side top -fill x\n\
\n\
frame $w.bar.rew\n\
button $w.bar.rew.b -bitmap rew -borderwidth 1 -relief raised \\\n\
-highlightthickness 0 -font $f -command \"$self rewind\"\n\
$balloon_ balloon_for $w.bar.rew.b \"rewind\" 1000\n\
frame $w.bar.rew.f -height 5 -relief sunken \\\n\
-borderwidth 1\n\
pack $w.bar.rew.b -side top -fill both -expand true\n\
pack $w.bar.rew.f -side top -fill x\n\
\n\
frame $w.bar.ff\n\
button $w.bar.ff.b -bitmap ff -borderwidth 1 -relief raised \\\n\
-highlightthickness 0 -font $f -command \"$self fast_fwd\"\n\
$balloon_ balloon_for $w.bar.ff.b \"fast forward\" 1000\n\
frame $w.bar.ff.f -height 5 -relief sunken \\\n\
-borderwidth 1\n\
pack $w.bar.ff.b -side top -fill both -expand true\n\
pack $w.bar.ff.f -side top -fill x\n\
\n\
\n\
pack $w.bar.rate -side right -fill y\n\
pack $w.bar.timerVal -side right -pady 0 \\\n\
-ipady 1 -padx 1 -fill y\n\
pack $w.bar.ff -side right -padx 1 -pady 1 -fill both -expand true\n\
pack $w.bar.run -side right -padx 1 -pady 1 -fill both -expand true\n\
pack $w.bar.stop -side right -padx 1 -pady 1 -fill both -expand true\n\
pack $w.bar.back -side right -padx 1 -pady 1 -fill both -expand true\n\
pack $w.bar.rew -side right -padx 1 -pady 1 -fill both -expand true\n\
\n\
pack $w.bar -fill x -expand 1 -side right\n\
$self instvar prevbutton\n\
set prevbutton $w.bar.stop\n\
\n\
$self highlight $w.bar.stop 1\n\
}\n\
\n\
Animator instproc toggle-edit { view tb db1 db2 } {\n\
$self instvar enable_edit_ balloon_\n\
set enable_edit_ [expr !$enable_edit_]\n\
if {$enable_edit_} {\n\
$self clear_view_bind $view\n\
$self editview_bind $view\n\
$tb configure -relief sunken\n\
$db1 configure -state normal\n\
$db2 configure -state normal\n\
$tb configure -bitmap \"netview\"\n\
$balloon_ balloon_for $db1 \"NodeUp (enabled)\"\n\
$balloon_ balloon_for $db2 \"NodeDown (enabled)\"\n\
} else {\n\
$self clear_editview_bind $view\n\
$self view_bind $view\n\
$tb configure -relief raised\n\
$db1 configure -state disabled\n\
$db2 configure -state disabled\n\
$tb configure -bitmap \"netedit\"\n\
$balloon_ balloon_for $db1 \"NodeUp (disabled)\"\n\
$balloon_ balloon_for $db2 \"NodeDown (disabled)\"\n\
}\n\
}\n\
\n\
Animator instproc build-zoombar {view w mainW} {\n\
$self instvar magnification viewOffset balloon_\n\
\n\
set magnification 1.0\n\
set viewOffset(x) 0.0\n\
set viewOffset(y) 0.0\n\
\n\
frame $w.f\n\
pack $w.f -side top\n\
button $w.f.b1 -bitmap \"zoomin\" -command \"$view zoom 1.6\" \\\n\
-highlightthickness 0 -borderwidth 1\n\
pack $w.f.b1 -side top -ipady 3\n\
button $w.f.b2 -bitmap \"zoomout\" -command \"$view zoom 0.625\" \\\n\
-highlightthickness 0 -borderwidth 1\n\
pack $w.f.b2 -side top -ipady 3\n\
\n\
$self instvar enable_edit_\n\
set enable_edit_ 0\n\
button $w.f.b3 -bitmap \"netedit\" -command \\\n\
\"$self toggle-edit $view $w.f.b3 $w.f.b4 $w.f.b5\" \\\n\
-highlightthickness 1 -borderwidth 1\n\
pack $w.f.b3 -side top -ipady 3\n\
\n\
$self instvar netModel \n\
button $w.f.b4 -bitmap \"nodeup\" -command \\\n\
\"$netModel incr-nodesize 1.1\" -state disabled\n\
button $w.f.b5 -bitmap \"nodedown\" -command \\\n\
\"$netModel decr-nodesize 1.1\" -state disabled\n\
pack $w.f.b4 -side top -ipady 3\n\
pack $w.f.b5 -side top -ipady 3\n\
\n\
$balloon_ balloon_for $w.f.b1 \"ZoomIn\"\n\
$balloon_ balloon_for $w.f.b2 \"ZoomOut\"\n\
$balloon_ balloon_for $w.f.b3 \"Edit/View\"\n\
$balloon_ balloon_for $w.f.b4 \"NodeUp (disabled)\"\n\
$balloon_ balloon_for $w.f.b5 \"NodeDown (disabled)\"\n\
\n\
if {[winfo name $mainW] != \"view\"} {\n\
button $w.f.b6 -bitmap \"eject\" -command \"destroy $mainW\" \\\n\
-highlightthickness 0 -borderwidth 1\n\
pack $w.f.b6 -side top -ipady 3\n\
}\n\
}\n\
\n\
Animator instproc setCurrentTime {time} {\n\
$self tkvar nowDisp\n\
set nowDisp $time\n\
}\n\
\n\
Animator instproc build-slider {w} {\n\
$self instvar timeslider timeslider_width timeslider_tag \\\n\
timeslider_pos nowDisp \n\
\n\
frame $w.f -borderwidth 2 -relief groove\n\
pack $w.f -side top -fill x -expand 1\n\
set timeslider(height) 12\n\
set timeslider(swidth) 32\n\
set timeslider(width) 32\n\
set timeslider_width($w.f.c) 32\n\
set timeslider(canvas) $w.f.c\n\
lappend timeslider(canvas_set) $timeslider(canvas)\n\
set timeslider(frame) $w\n\
canvas $w.f.c -height $timeslider(height) -width 300 \\\n\
-background white -highlightthickness 0\n\
pack $w.f.c -side left -fill x -expand 1 -padx 0 -pady 0\n\
\n\
label $w.f.c.b -bitmap time -highlightthickness 0 -borderwidth 1 \\\n\
-relief raised\n\
set timeslider_tag($w.f.c) [$w.f.c create window \\\n\
[expr $timeslider(swidth)/2] 0 -window $w.f.c.b \\\n\
-height 12 -width $timeslider(swidth) -anchor n]\n\
set timeslider_pos($w.f.c) 0\n\
bind $w.f.c <ButtonPress-1> \"$self timeslidertrough $w.f.c %x %y\"\n\
bind $w.f.c.b <ButtonPress-1> \"$self timesliderpress $w.f.c %x %y;break\"\n\
bind $w.f.c.b <ButtonRelease-1> \"$self timesliderrelease $w.f.c %x %y\"\n\
bind $w.f.c.b <B1-Motion> \"$self timeslidermotion $w.f.c %x %y\"\n\
bind $w.f.c <Configure> \"$self timeticks $w.f.c\"\n\
}\n\
\n\
Animator instproc timeticks { wfc } {\n\
$self instvar timeslider mintime range timeslider_width \\\n\
timeslider_tag maxtime mid_\n\
set timeslider(canvas) $wfc\n\
\n\
set st [lindex [split $wfc \".\"] 2]\n\
if { [string compare $st \"ctrl\"] == 0 } {\n\
set width [winfo width $timeslider(canvas)]\n\
} else {\n\
if { [string compare $st \"slider\"] == 0 } {\n\
set namgraphname_ [lindex [split $wfc \".\"] 1]\n\
set width [winfo width .$namgraphname_.main.c]\n\
} else {                    \n\
set width [winfo width $tlw_.$st.main.c]\n\
}\n\
}\n\
if {$width == $timeslider_width($wfc)} { return }\n\
set timeslider(width) $width\n\
set timeslider_width($wfc) $width\n\
$timeslider(canvas) delete ticks\n\
\n\
pack forget $timeslider(canvas)\n\
update\n\
update idletasks\n\
pack $timeslider(canvas) -side left -fill x -expand 1 -padx 0 -pady 0\n\
\n\
set width [winfo width $wfc]\n\
\n\
set x [expr $timeslider(swidth)/2]\n\
set tickIncr [expr $range / ($width-$timeslider(swidth))]\n\
if {$range == 0} {\n\
set intertick [expr ($width - $timeslider(swidth)) / 10]\n\
} else {\n\
set intertick [expr ($width-$timeslider(swidth))/(10 * $range)]\n\
}\n\
if {$intertick < 2} {\n\
set intertick 2\n\
}\n\
for {set t $mintime} {$t < ($range+$mintime)} {set t [expr $t+$intertick*$tickIncr]} {\n\
set intx [expr int($x)]\n\
$timeslider(canvas) addtag ticks withtag \\\n\
[$timeslider(canvas) create line \\\n\
$intx [expr $timeslider(height)/2] \\\n\
$intx $timeslider(height)]\n\
set x [expr $x+$intertick]\n\
}\n\
set x [expr $timeslider(swidth)/2]\n\
if {$range == 0} {\n\
set intertick [expr $width - $timeslider(swidth)]\n\
} else {\n\
set intertick [expr ($width-$timeslider(swidth))/($range)]\n\
}\n\
if {$intertick < 20} {\n\
set intertick 20\n\
}\n\
for {set t $mintime} {$t < ($range+$mintime)} {set t [expr $t+$intertick*$tickIncr]} {\n\
set intx [expr int($x)]\n\
$timeslider(canvas) addtag ticks withtag \\\n\
[$timeslider(canvas) create line \\\n\
$intx 0 $intx $timeslider(height)]\n\
set x [expr $x+$intertick]\n\
}\n\
\n\
set wfc_width [winfo width $wfc]\n\
\n\
if {$maxtime > 0} {\n\
set x [expr ($wfc_width-$timeslider(swidth))*$now/$maxtime]\n\
} else {\n\
set x [expr ($wfc_width-$timeslider(swidth))*$now]\n\
}\n\
\n\
$wfc coords $timeslider_tag($wfc) [expr $x + $timeslider(swidth)/2] 0\n\
}\n\
\n\
\n\
Animator instproc getmainslidermodel {} {\n\
$self instvar mslider\n\
return $mslider\n\
}\n\
\n\
Animator instproc setsliderPressed {v} {\n\
$self instvar sliderPressed\n\
set sliderPressed $v\n\
}\n\
\n\
Animator instproc getrunning {} {\n\
$self instvar running\n\
return $running\n\
}\n\
\n\
Animator instproc timesliderset {t} {\n\
$self instvar mslider\n\
$mslider setcurrenttime $t\n\
return\n\
}\n\
\n\
Animator instproc timesliderpress {w x y} {\n\
$self instvar timeslider sliderPressed \n\
set timeslider(oldpos) $x\n\
$w.b configure -relief sunken\n\
set sliderPressed 1\n\
}\n\
\n\
Animator instproc timeslidertrough {w x y} {\n\
$self instvar timeslider sliderPressed timeslider_pos\n\
if {$timeslider_pos($w)>$x} {\n\
$self rewind\n\
} else {\n\
$self fast_fwd\n\
}\n\
}\n\
\n\
Animator instproc timeslidermotion {wc x y} {\n\
$self instvar timeslider mintime range timeslider_tag \\\n\
timeslider_pos timeslider_width\n\
$self tkvar nowDisp\n\
\n\
set diff [expr $x - $timeslider(oldpos)]\n\
\n\
set timeslider(canvas) $wc\n\
\n\
$timeslider(canvas) move $timeslider_tag($wc) \\\n\
$diff 0\n\
\n\
set timeslider_pos($wc) [expr $timeslider_pos($wc) + $diff]\n\
if {$timeslider_pos($wc)<0} {\n\
$timeslider(canvas) move $timeslider_tag($wc) \\\n\
[expr 0 - $timeslider_pos($wc)] 0\n\
set timeslider_pos($wc) 0\n\
}\n\
\n\
if {$timeslider_pos($wc)>[expr $timeslider_width($wc)-$timeslider(swidth)]} {\n\
$timeslider(canvas) move $timeslider_tag($wc) \\\n\
[expr ($timeslider_width($wc) - $timeslider(swidth)) - \\\n\
$timeslider_pos($wc)] 0\n\
\n\
set timeslider_pos($wc) [expr $timeslider_width($wc)-$timeslider(swidth)]\n\
}\n\
\n\
set tick 0\n\
catch {\n\
set tick [expr 1000.0*$timeslider_pos($wc)/($timeslider_width($wc)-$timeslider(swidth))]\n\
}\n\
set now [expr ($tick * $range) / 1000. + $mintime]\n\
set nowDisp [format %.6f $now]\n\
set timeslider(tick) $tick\n\
\n\
$self timesliderset $now\n\
}\n\
\n\
Animator instproc timesliderrelease {w x y} {\n\
$self instvar timeslider sliderPressed running\n\
\n\
$self timeslidermotion $w $x $y\n\
$w.b configure -relief raised\n\
$self slidetime $timeslider(tick) 1\n\
set sliderPressed 0\n\
if $running {\n\
$self renderFrame\n\
}\n\
}\n\
\n\
Animator instproc nam-relayout {} {\n\
$self tkvar ITERATIONS KCa KCr Recalc \n\
$self instvar now netModel\n\
\n\
$netModel do_relayout $ITERATIONS $KCa $KCr $Recalc\n\
$self settime $now\n\
update idletasks\n\
}\n\
\n\
Animator instproc set-layout-params { iter kca kcr } {\n\
$self tkvar ITERATIONS KCa KCr \n\
set ITERATIONS $iter\n\
set KCa $kca \n\
set KCr $kcr\n\
}\n\
\n\
Animator instproc build-layout {w} {\n\
$self tkvar ITERATIONS KCa KCr Recalc\n\
$self instvar NETWORK_MODEL \n\
if {$NETWORK_MODEL != \"NetworkModel/Auto\"} {\n\
return\n\
}\n\
\n\
set f [smallfont]\n\
label $w.label -text \"Auto layout:\" -font $f\n\
label $w.label_ca -text \"Ca\" -font $f\n\
entry $w.inputca -width 6 -relief sunken \\\n\
-textvariable [$self tkvarname KCa] -font $f\n\
label $w.label_cr -text \"Cr\" -font $f\n\
entry $w.inputcr -width 6 -relief sunken \\\n\
-textvariable [$self tkvarname KCr] -font $f\n\
label $w.label_iter -text \"Iterations\" -font $f\n\
entry $w.inputiter -width 6 -relief sunken \\\n\
-textvariable [$self tkvarname ITERATIONS]\n\
\n\
set Recalc 1\n\
checkbutton $w.recalc -text Recalc -onvalue 1 -offvalue 0 \\\n\
-variable [$self tkvarname Recalc] -font $f \\\n\
-highlightthickness 0\n\
\n\
$self instvar netModel\n\
button $w.reset -text reset -relief raised -font $f \\\n\
-command \"$netModel reset\"\n\
\n\
button $w.relayout -text re-layout -relief raised -font $f \\\n\
-command \"$self nam-relayout\"\n\
\n\
pack $w.label -side left -padx 1 -pady 1\n\
pack $w.label_ca -side left -padx 1 -pady 1 -fill x\n\
pack $w.inputca -side left -padx 1 -pady 1 -fill x\n\
pack $w.label_cr -side left -padx 1 -pady 1 -fill x\n\
pack $w.inputcr -side left -padx 1 -pady 1 -fill x\n\
\n\
pack $w.label_iter -side left -padx 1 -pady 1 -fill x\n\
pack $w.inputiter -side left -padx 1 -pady 1 -fill x\n\
pack $w.recalc -side left -padx 1 -pady 1 -fill both\n\
pack $w.reset -side right -padx 1 -pady 1 -fill both\n\
pack $w.relayout -side right -padx 1 -pady 1 -fill both\n\
\n\
bind $w.inputca <Return> \" $self nam-relayout\"\n\
bind $w.inputcr <Return> \" $self nam-relayout\"\n\
bind $w.inputiter <Return> \"$self nam-relayout\"\n\
}\n\
\n\
Animator instproc build-annotation { w } {\n\
$self tkvar showpanel\n\
$self instvar annoBox annoBoxHeight annoJump\n\
frame $w.spaceral -borderwidth 0 -highlightthickness 0 \\\n\
-height 0 -width 10\n\
pack $w.spaceral -side top -padx 0 -pady 0\n\
frame $w.f -borderwidth 0 -highlightthickness 0\n\
frame $w.f.f\n\
\n\
set annoJump 0\n\
set annoBoxHeight 3\n\
listbox $w.f.f.a -xscrollcommand \"$w.f.f.ah set\" \\\n\
-yscrollcommand \"$w.f.f2.av set\" -height $annoBoxHeight \\\n\
-selectmode single \n\
pack $w.f.f.a -fill both -side top -expand true \n\
set annoBox $w.f.f.a\n\
\n\
scrollbar $w.f.f.ah -orient horizontal -width 10 -borderwidth 1 \\\n\
-command \"$w.f.f.a xview\"\n\
$w.f.f.ah set 0.0 1.0\n\
pack $w.f.f.ah -side bottom -fill x\n\
\n\
frame $w.f.f2\n\
pack $w.f.f2 -side left -fill y\n\
scrollbar $w.f.f2.av -orient vertical -width 10 -borderwidth 1 \\\n\
-command \"$w.f.f.a yview\"\n\
$w.f.f2.av set 0.0 1.0\n\
pack $w.f.f2.av -side top -fill y -expand true\n\
pack $w.f.f -side left -fill both -expand true\n\
trace variable showpanel(annotate) w \\\n\
\"$self displaypanel $w.f $w.spaceral annotate top both true\"\n\
\n\
bind $w.f.f.a <Double-ButtonPress-1> \"$self jump_to_annotation $w.f.f.a\"\n\
bind $w.f.f.a <ButtonPress-3> \"$self popup_annotation $w \\%x \\%y\"\n\
}\n\
\n\
Animator instproc displaypanel {panel after name side fill expand args} {\n\
$self tkvar showpanel\n\
\n\
if {$showpanel($name) == 1} {\n\
set str \"pack $panel -side $side -fill $fill -expand $expand\"\n\
if {$after!=\"\"} {\n\
set str \"$str -after $after\"\n\
}\n\
eval $str\n\
} else {\n\
pack forget $panel\n\
}\n\
}\n\
\n\
Animator instproc closepanel { name } {\n\
$self tkvar showpanel\n\
set showpanel($name) 0\n\
}\n\
\n\
Animator instproc highlight {w mode} {\n\
$self instvar prevbutton\n\
if {$mode==1} {\n\
$prevbutton.b configure -relief raised\n\
$prevbutton.f configure -background [option get . background Nam]\n\
set prevbutton $w\n\
}\n\
$w.f configure -background seagreen\n\
$w.f configure -relief sunken\n\
}\n\
\n\
Animator instproc trace_running_handler { w args } { \n\
$self instvar direction running\n\
\n\
if {$running == 0} {\n\
$self highlight $w.bar.stop 1 \n\
} elseif {($direction == 1)} {\n\
$self highlight $w.bar.run 1\n\
} else {\n\
$self highlight $w.bar.back 1\n\
}\n\
}\n\
\n\
Animator instproc window_bind {w} {\n\
bind $w <q> \"$self done\"\n\
bind $w <Q> \"$self all_done\"\n\
bind $w <Control-w> \"$self done\"\n\
bind $w <Control-W> \"$self done\"\n\
bind $w <Control-q> \"[AnimControl instance] done\"\n\
bind $w <Control-Q> \"[AnimControl instance] done\"\n\
bind $w <Control-c> \"$self done\"\n\
\n\
wm protocol $w WM_DELETE_WINDOW \"$self done\"\n\
\n\
bind $w <b> \"$self back_step\"\n\
bind $w <B> \"$self back_step\"\n\
\n\
bind $w <c> \"$self play 1\"\n\
bind $w <C> \"$self play 1\"\n\
bind $w <f> \"$self fast_fwd\"\n\
bind $w <F> \"$self fast_fwd\"\n\
bind $w <n> \"$self next_event\"\n\
bind $w <N> \"$self next_event\"\n\
bind $w <p> \"$self stop 1\"\n\
bind $w <P> \"$self stop 1\"\n\
bind $w <r> \"$self rewind\"\n\
bind $w <R> \"$self rewind\"\n\
bind $w <u> \"$self time_undo\"\n\
bind $w <U> \"$self time_undo\"\n\
bind $w <x> \"$self rate_undo\"\n\
bind $w <X> \"$self rate_undo\"\n\
bind $w <period> \"$self change_rate 1\"\n\
bind $w <greater> \"$self change_rate 1\"\n\
bind $w <comma> \"$self change_rate 0\"\n\
bind $w <less> \"$self change_rate 0\"\n\
}\n\
\n\
Animator instproc view_bind {netView} {\n\
bind $netView <space> \"$self toggle_pause\"\n\
bind $netView <Control-d> \"$self done\"\n\
bind $netView <Return> \"$self single_step\"\n\
bind $netView <BackSpace> \"$self back_step\"\n\
bind $netView <Delete> \"$self back_step\"\n\
bind $netView <0> \"$self reset\"\n\
\n\
bind $netView <ButtonPress-1> \"$self start_info $netView \\%x \\%y left\"\n\
bind $netView <ButtonPress-3> \"$self start_info $netView \\%x \\%y right\"\n\
bind $netView <ButtonPress-2> \"$self view_drag_start $netView \\%x \\%y\"\n\
bind $netView <B2-Motion> \"$self view_drag_motion $netView \\%x \\%y\"\n\
}\n\
\n\
Animator instproc clear_view_bind { netView } {\n\
bind $netView <ButtonPress-1> \"\"\n\
bind $netView <ButtonPress-3> \"\"\n\
bind $netView <ButtonPress-2> \"\"\n\
bind $netView <B2-Motion> \"\"\n\
}\n\
\n\
Animator instproc tracehooktcl { e } {\n\
\n\
$self notifyObservers $e\n\
}\n\
\n\
Animator instproc update_statsview { s event } {\n\
\n\
$self instvar colorset highest_seq timeslider maxtime colorname subView \\\n\
subViewRange plotdatax plotdatay plotmark\n\
\n\
set s_len [string length $s]\n\
set session_id [string index $s [expr $s_len-1]]\n\
\n\
set sid [get_trace_item \"-S\" $event]\n\
\n\
if {$sid != $session_id} {return}\n\
\n\
set extinfo [get_trace_item \"-x\" $event]\n\
set time [get_trace_item \"-t\" $event]\n\
set seqno [lindex $extinfo 2]\n\
set colorid [get_trace_item \"-a\" $event]\n\
set mid [get_trace_item \"-m\" $event]\n\
\n\
if { $mid == \"\" } {return}\n\
\n\
set cnt 0\n\
while {[info exists plotdatax($sid.$cnt)]} {\n\
$self plot_xy $s.main.c $time $seqno 0 $maxtime 0 $highest_seq($sid) \\\n\
$colorid $plotmark($sid.$mid) 0\n\
incr cnt\n\
}\n\
\n\
foreach subviews $subView(#$session_id) {\n\
if {![winfo exists $subviews]} {continue}\n\
\n\
set minx [lindex $subViewRange($subviews) 0]\n\
set maxx [lindex $subViewRange($subviews) 1]\n\
set miny [lindex $subViewRange($subviews) 2]\n\
set maxy [lindex $subViewRange($subviews) 3]\n\
$self plot_xy $subviews.main.c $time $seqno $minx $maxx \\\n\
$miny $maxy $colorid $plotmark($sid.$mid) 1\n\
\n\
}\n\
}\n\
\n\
\n\
Animator instproc new_waitingtext { msg } {\n\
$self instvar windows \n\
$windows(title) configure -text \"Please wait - $msg\"\n\
update\n\
}\n\
\n\
Animator instproc restore_waitingtext {} {\n\
$self instvar windows nam_name\n\
$windows(title) configure -text $nam_name\n\
}\n\
\n\
Animator instproc active_sessions {} {\n\
$self instvar analysis_ready colorset colorindex session_id colorname \\\n\
netModel analysis_flag windows nam_name tlw_ cache_ready\n\
\n\
if {[string compare $analysis_flag \"0\"] == 0} {\n\
set analysis_flag 1\n\
}\n\
\n\
set w $tlw_.activesessions\n\
\n\
if {[winfo exists $w]} {\n\
raise $w\n\
return\n\
} \n\
$windows(title) configure -text \"Please wait ... \"\n\
update\n\
if { $analysis_ready == 1 && $cache_ready == 0 } {\n\
$self cache_plot\n\
set cache_ready 1\n\
}\n\
$windows(title) configure -text $nam_name\n\
\n\
\n\
toplevel $w\n\
\n\
wm title $w \"Current Active Sessions\"\n\
\n\
bind $w <Control-w> \"destroy $w\"\n\
bind $w <Control-W> \"destroy $w\"\n\
bind $w <Control-q> \"[AnimControl instance] done\"\n\
bind $w <Control-Q> \"[AnimControl instance] done\"\n\
\n\
frame $w.f \n\
pack $w.f -side top\n\
\n\
label $w.f.label_left -text \"LEGEND\"\n\
label $w.f.label_right -text \"SESSIONS\"\n\
\n\
for {set i 0} { $i < $colorindex } { incr i} {\n\
\n\
label $w.f.label_left$i -text \"    \" -bg $colorname($i);\n\
set stats_title $session_id($i)\n\
button $w.f.button$i -text $session_id($i) \\\n\
-command \"$self make_mainwin \\\"$i\\\"\"\n\
\n\
}\n\
\n\
grid config $w.f.label_left -column 0 -row 0 -sticky \"n\"\n\
grid config $w.f.label_right -column 1 -row 0 -sticky \"n\"\n\
\n\
for {set i 0} { $i < $colorindex } { incr i} {\n\
grid config $w.f.label_left$i -column 0 -row [expr $i+1] \\\n\
-sticky \"snew\"\n\
grid config $w.f.button$i -column 1 -row [expr $i+1] \\\n\
-sticky \"snew\"\n\
}\n\
}\n\
\n\
Animator instproc auto_legend {} {\n\
\n\
$self instvar Mcnt plotmarks colorname filter_id filtercolor_id colorindex tlw_ \n\
\n\
set w $tlw_.autolegend\n\
\n\
if {[winfo exists $w]} {\n\
raise $w\n\
return\n\
}\n\
\n\
toplevel $w\n\
wm title $w \"Current Filter Legend\"\n\
\n\
bind $w <Control-w> \"destroy $w\"\n\
bind $w <Control-W> \"destroy $w\"\n\
bind $w <Control-q> \"[AnimControl instance] done\"\n\
bind $w <Control-Q> \"[AnimControl instance] done\"\n\
\n\
frame $w.f\n\
pack $w.f -side top\n\
\n\
label $w.f.label_left -text \"LEGEND\"\n\
label $w.f.label_right -text \"EXPLANATION\"\n\
\n\
for {set i 0} { $i < $Mcnt } { incr i} {\n\
\n\
\n\
\n\
label $w.f.label_left$i -bitmap $plotmarks($i) -fg $colorname($filtercolor_id($i))\n\
\n\
label $w.f.button$i -text $filter_id($i)\n\
\n\
}\n\
\n\
grid config $w.f.label_left -column 0 -row 0 -sticky \"n\"\n\
grid config $w.f.label_right -column 1 -row 0 -sticky \"n\"\n\
\n\
for {set i 0} { $i < $Mcnt } { incr i} {\n\
\n\
grid config $w.f.label_left$i -column 0 -row [expr $i+1] \\\n\
-sticky \"snew\"\n\
grid config $w.f.button$i -column 1 -row [expr $i+1] \\\n\
-sticky \"snew\"\n\
}\n\
\n\
}\n\
\n\
Animator instproc ScrolledCanvas { c width height region } {\n\
frame $c\n\
canvas $c.canvas -width $width -height $height \\\n\
-scrollregion $region \\\n\
-xscrollcommand [list $c.xscroll set] \\\n\
-yscrollcommand [list $c.yscroll set]\n\
scrollbar $c.xscroll -orient horizontal \\\n\
-command [list $c.canvas xview]\n\
scrollbar $c.yscroll -orient vertical \\\n\
-command [list $c.canvas yview]\n\
pack $c.xscroll -side bottom -fill x\n\
pack $c.yscroll -side right -fill y\n\
pack $c.canvas -side left -fill both -expand true\n\
pack $c -side top -fill both -expand true\n\
return $c.canvas\n\
}\n\
\n\
Animator instproc build.m0 { w } {\n\
bind $w <Configure> \"$self xtimeticks $w\"\n\
}\n\
\n\
Animator instproc xtimeticks { w } {\n\
$self instvar timeslider mintime range timeslider_width\n\
\n\
set width [winfo width $w]\n\
set height [winfo height $w]\n\
\n\
$w delete ticks\n\
\n\
set x [expr $timeslider(swidth)/2]\n\
set intertick [expr ($width-$timeslider(swidth))/(10 * $range)]\n\
for {set t $mintime} {$t < ($range+$mintime)} {set t [expr $t+0.1]} {\n\
set intx [expr int($x)]\n\
$w addtag ticks withtag \\\n\
[$w create line \\\n\
$intx [expr $timeslider(height)/2 + $height*9/10] $intx [expr $timeslider(height) + $height*9/10]]\n\
set x [expr $x+$intertick]\n\
}\n\
\n\
set orx [expr $timeslider(swidth)/2]\n\
$w addtag ticks withtag \\\n\
[$w create line $orx [expr $timeslider(height)+$height*9/10] $x [expr $timeslider(height)+$height*9/10]]\n\
$w addtag ticks withtag \\\n\
[$w create line $orx [expr $timeslider(height)+$height*9/10] $orx 0]]\n\
\n\
set x [expr $timeslider(swidth)/2]\n\
set intertick [expr ($width-$timeslider(swidth))/($range)]\n\
for {set t $mintime} {$t < ($range+$mintime)} {set t [expr $t+1]} {\n\
set intx [expr int($x)]\n\
$w addtag ticks withtag \\\n\
[$w create line \\\n\
$intx [expr $timeslider(height) + $height*9/10] $intx [expr $height*9/10]]\n\
set x [expr $x+$intertick]\n\
}\n\
}\n\
\n\
\n\
Animator instproc make_mainwin { sid } {\n\
$self tkvar model_ \n\
$self instvar session_id\n\
$model_($sid) startview $session_id($sid)\n\
}\n\
\n\
\n\
\n\
Animator instproc nam_analysis { tracefile } {\n\
$self instvar analysis_OK analysis_ready trace cache_ready count\n\
\n\
set stream [new NamStream $tracefile]\n\
set line [$stream gets]\n\
set time [get_trace_item \"-t\" $line]\n\
set count 0\n\
\n\
\n\
while {([$stream eof]==0)&&([string compare $time \"*\"]!=0)} {\n\
set line [$stream gets]\n\
set time [get_trace_item \"-t\" $line] \n\
}\n\
\n\
while {([$stream eof]==0)&&([string compare $time \"*\"]==0) } {\n\
set cmd [lindex $line 0]\n\
if [regexp {^\\#} $cmd] {\n\
continue\n\
}\n\
\n\
switch \"$cmd\" {\n\
\"V\" {\n\
$self handle_version $line\n\
}\n\
\"N\" {\n\
$self handle_analysis $line\n\
}\n\
\"c\" {\n\
$self handle_colorname $line\n\
}\n\
}\n\
set count [expr $count + 1]\n\
set line [$stream gets]\n\
set time [get_trace_item \"-t\" $line]\n\
}\n\
$stream close\n\
\n\
if { $count==0 } {\n\
puts \"*** !!! ***\"\n\
puts \"nam cannot recognize the trace file $tracefile\"\n\
puts \"Please make sure that the file is not empty and it is a nam trace\"\n\
puts \"***********\"\n\
exit\n\
}\n\
\n\
if { $analysis_OK == 0 } { \n\
puts \"You are using the tracefile format older than 1.0a5\"\n\
puts \"which will not allow you to run namgraph\"\n\
return\n\
}\n\
\n\
set cache_ready 0\n\
}\n\
\n\
Animator instproc cache_plot { } {\n\
$self instvar tracefile plotdatax plotdatay plotmark plotmarks plotcolor\n\
\n\
if ![info exists plotmarks] {\n\
set plotmarks(0) mark1\n\
set plotmarks(1) mark2\n\
set plotmarks(2) mark3\n\
set plotmarks(3) mark4\n\
set plotmarks(4) mark5\n\
set plotmarks(5) mark6\n\
set plotmarks(6) mark7\n\
set plotmarks(7) mark8\n\
set plotmarks(8) mark1\n\
set plotmarks(9) mark2\n\
set plotmarks(10) mark3\n\
set plotmarks(11) mark4\n\
set plotmarks(12) mark5\n\
set plotmarks(13) mark6\n\
set plotmarks(14) mark7\n\
set plotmarks(15) mark8\n\
\n\
}\n\
\n\
set file [new NamStream $tracefile]\n\
\n\
$self tkvar model_\n\
\n\
while {[$file eof]==0} {\n\
set line [$file gets]\n\
set time [get_trace_item \"-t\" $line]\n\
\n\
if {[string compare $time \"*\"]==0 } {continue}\n\
\n\
set Sid [get_trace_item \"-S\" $line]\n\
set mid [get_trace_item \"-m\" $line]\n\
set pid [get_trace_item \"-p\" $line]\n\
set fid [get_trace_item \"-f\" $line]\n\
set yvalset [get_trace_item \"-y\" $line]\n\
\n\
set yval [lindex $yvalset 0]\n\
set ymark [lindex $yvalset 1]\n\
\n\
if { $mid == \"\" } {continue}\n\
\n\
set plotmark($Sid.$mid) $plotmarks($mid)\n\
set plotcolor($Sid.$mid) $fid\n\
\n\
\n\
if ![info exists model_($Sid)] {\n\
set model_($Sid) [new NamgraphModel $Sid $self]\n\
$self addObserver $model_($Sid)\n\
}\n\
\n\
set current_model $model_($Sid)\n\
$current_model adddata $self $mid $time $yval $ymark\n\
\n\
}\n\
$file close\n\
}\n\
\n\
Animator instproc handle_analysis { line } {\n\
\n\
$self instvar session_id filter_id colorname highest_seq filtercolor_id \\\n\
ymark\n\
\n\
set index [get_trace_item \"-S\" $line]\n\
set findex [get_trace_item \"-F\" $line]\n\
set title [get_trace_item \"-n\" $line]\n\
set hseq [get_trace_item \"-h\" $line]\n\
set mindex [get_trace_item \"-M\" $line]\n\
set groupm [get_trace_item \"-m\" $line]\n\
set proto [get_trace_item \"-p\" $line]\n\
\n\
if { $index != \"\" && $title != \"\" } {\n\
set session_id($index) $title\n\
set proto_id($index) $proto\n\
}\n\
if { $index != \"\" && $hseq != \"\" } {\n\
set highest_seq($index) $hseq\n\
}\n\
if { $findex != \"\" } {\n\
set filter_id($mindex) $title\n\
set filtercolor_id($mindex) $findex\n\
}\n\
\n\
}\n\
\n\
Animator instproc handle_colorname { line } {\n\
$self instvar colorname\n\
set index [get_trace_item \"-i\" $line] \n\
set colorn [get_trace_item \"-n\" $line]\n\
set colorname($index) $colorn\n\
}\n\
\n\
Animator instproc handle_version { line } {\n\
\n\
$self instvar analysis_OK nam_version analysis_ready colorindex \\\n\
highest_seq Mcnt\n\
\n\
set nam_version [get_trace_item \"-v\" $line]\n\
if { $nam_version >= \"1.0a5\" } {\n\
set analysis_OK 1\n\
}\n\
set analysis_ready [get_trace_item \"-a\" $line]\n\
set colorindex [get_trace_item \"-c\" $line]\n\
set Mcnt [get_trace_item \"-M\" $line]\n\
}\n\
\n\
Animator instproc viewgraph { object graphtype tracefile } {\n\
$self instvar netView now vslider windows nam_name graphs tlw_ \n\
\n\
if {$object==\"\"} {return}\n\
set graphtrace [new Trace $tracefile $self]\n\
set netgraph \"\"\n\
switch [lindex $object 0] {\n\
l {\n\
set netgraph [new LinkNetworkGraph]\n\
switch $graphtype {\n\
\"bw\" {\n\
$netgraph bw [lindex $object 1] [lindex $object 2]\n\
}\n\
\"loss\" {\n\
$netgraph loss [lindex $object 1] [lindex $object 2]\n\
}\n\
}\n\
}\n\
f {\n\
set netgraph [new FeatureNetworkGraph]\n\
$netgraph feature [lindex $object 1] [lindex $object 2] [lindex $object 3]\n\
}\n\
}\n\
\n\
if {$netgraph==\"\"} {\n\
return\n\
}\n\
set name [lindex $object 0]_[lindex $object 1]_[lindex $object 2]_$graphtype\n\
\n\
if {[winfo exists $tlw_.graph.f$name]==1} {\n\
return\n\
}\n\
\n\
$windows(title) configure -text \"Please wait - reading tracefile...\"\n\
update\n\
set maxtime [$graphtrace maxtime]\n\
set mintime [$graphtrace mintime]\n\
$graphtrace connect $netgraph\n\
$netgraph timerange $mintime $maxtime\n\
\n\
$graphtrace settime $maxtime 1\n\
\n\
set w $tlw_.graph\n\
if {[winfo exists $w]==0} {\n\
frame $w \n\
pack $w -side top -fill x -expand true -after [$vslider frame]\n\
}\n\
lappend graphs $netgraph\n\
frame $w.f$name -borderwidth 2 -relief groove\n\
pack $w.f$name -side top -expand true -fill both\n\
label $w.f$name.pr -bitmap pullright -borderwidth 1 -relief raised\n\
bind $w.f$name.pr <Enter> \\\n\
\"$self viewgraph_label \\\"[$self viewgraph_name $object $graphtype]\\\" \\\n\
$w.f$name $w.f$name.pr $netgraph\"\n\
pack $w.f$name.pr -side left\n\
$netgraph view $w.f$name.view\n\
\n\
$netgraph settime $now\n\
\n\
pack $w.f$name.view -side left -expand true \\\n\
-fill both\n\
frame $w.f$name.l2 -width [expr [$vslider swidth]/2] -height 30\n\
pack $w.f$name.l2 -side left\n\
$windows(title) configure -text $nam_name\n\
}\n\
\n\
Animator instproc viewgraph_label {info win where netgraph} {\n\
$self instvar tlw_\n\
\n\
if {[winfo exists $win.lbl]==0} {\n\
frame $win.lbl -borderwidth 2 -relief groove\n\
button $win.lbl.hide -text \"Hide\" -relief raised -borderwidth 1 \\\n\
-highlightthickness 0 \\\n\
-command \"destroy $win;\\\n\
$self rm_list_entry graphs $netgraph;\\\n\
if {\\[winfo children $tlw_.graph\\]=={}} {destroy $tlw_.graph}\"\n\
pack $win.lbl.hide -side left\n\
label $win.lbl.l -text $info -font [smallfont]\n\
pack $win.lbl.l -side left\n\
}\n\
catch {\n\
pack $win.lbl -side left -after $where -fill y\n\
pack forget $where\n\
bind $win.lbl <Leave> \\\n\
\"pack $where -side left -before $win.lbl;pack forget $win.lbl\"\n\
}\n\
}\n\
\n\
Animator instproc rm_list_entry {var value} {\n\
$self instvar $var\n\
set res \"\"\n\
set lst [set [set var]]\n\
foreach el $lst {\n\
if {[string compare $el $value]!=0} {\n\
lappend res $el\n\
}\n\
}\n\
set [set var] $res\n\
}\n\
\n\
Animator instproc viewgraph_name {name graphtype} {\n\
set type [lindex $name 0]\n\
switch $type {\n\
\"l\" {\n\
switch $graphtype {\n\
\"bw\" {\n\
return \"Bandwidth used on link [lindex $name 1]->[lindex $name 2]\"\n\
}\n\
\"loss\" {\n\
return \"Packets dropped on link [lindex $name 1]->[lindex $name 2]\"\n\
}\n\
}\n\
}\n\
\"f\" {\n\
return \"[lindex $name 2] [lindex $name 3]\"\n\
\n\
}\n\
}\n\
return unknown\n\
}\n\
\n\
\n\
Class Observer\n\
Observer set uniqueID_ 0\n\
\n\
Observer proc getid {} {\n\
set id [Observer set uniqueID_]\n\
Observer set uniqueID_ [expr $id + 1]\n\
return $id\n\
}\n\
\n\
Observer instproc init {} {\n\
\n\
$self instvar id_\n\
set id_ [Observer getid]\n\
}\n\
\n\
Observer instproc id {} {\n\
$self instvar id_\n\
return $id_\n\
}\n\
\n\
\n\
Observer instproc update {} {\n\
\n\
}\n\
\n\
Class Observable\n\
\n\
Observable instproc init {} {\n\
$self instvar observerlist_ \n\
set observerlist_ \"\"\n\
}\n\
\n\
\n\
Observable instproc addObserver { o } {\n\
$self instvar observerlist_\n\
\n\
set cnt 0\n\
set oid [$o id]\n\
foreach ob $observerlist_ {\n\
set obid [$ob id]\n\
if { $oid == $obid } {\n\
set cnt 1\n\
break;\n\
}\n\
}  \n\
\n\
if { $cnt == 0 } {\n\
lappend observerlist_ $o\n\
}\n\
\n\
}\n\
\n\
\n\
Observable instproc  deleteObserver { o } {\n\
\n\
$self instvar observerlist_\n\
set backlist_ \"\"\n\
set oid [$o id]\n\
foreach ob $observerlist_ {\n\
set obid [$ob id]\n\
if { $oid != $obid } {\n\
lappend backlist_ $ob\n\
} else {\n\
}\n\
}\n\
\n\
set observerlist_ $backlist_\n\
}\n\
\n\
\n\
Observable instproc notifyObservers { arg } {\n\
\n\
$self instvar observerlist_\n\
\n\
\n\
foreach ob $observerlist_ {\n\
\n\
if ![ catch { $ob info class } ] {\n\
\n\
$ob update $arg\n\
}   \n\
\n\
}\n\
}\n\
\n\
\n\
Observable instproc countObservers {} {\n\
$self instvar observerlist_\n\
set size [llength $observerlist_]\n\
return $size\n\
}\n\
\n\
NetworkModel/Wireless set Wpxmax_ 4\n\
NetworkModel/Wireless set Wpymax_ 4\n\
\n\
NetworkModel/Wireless instproc init { animator tracefile } {\n\
eval $self next $animator $tracefile\n\
NetworkModel/Wireless instvar Wpxmax_ Wpymax_\n\
$self set_range $Wpxmax_ $Wpymax_\n\
}\n\
\n\
NetworkModel/Wireless instproc set_range { width height } {\n\
$self instvar Wpxmax_ Wpymax_\n\
set Wpxmax_ $width\n\
set Wpymax_ $height\n\
}\n\
\n\
NetworkModel/Wireless instproc add_link { e } {\n\
\n\
set val [lrange $e 1 end]\n\
\n\
$self layout_link $val\n\
\n\
}\n\
\n\
\n\
Class NamgraphView -superclass Observer\n\
\n\
NamgraphView set WinWidth  600\n\
NamgraphView set WinHeight 400\n\
\n\
NamgraphView instproc init { title ob minx maxx miny maxy tag } {\n\
\n\
$self next\n\
\n\
$self instvar id_ title_ modelobj_ winname_ minx_ maxx_ miny_ maxy_\n\
$self instvar slidetag_ timeslider\n\
\n\
set title_ $title\n\
set modelobj_ $ob\n\
set slidetag_ $tag\n\
\n\
set winname_ .namgraph-$id_\n\
\n\
set minx_ $minx\n\
set miny_ $miny\n\
set maxx_ $maxx\n\
set maxy_ $maxy\n\
\n\
if { $slidetag_ != 1 } {\n\
set timeslider(swidth) [$modelobj_ set timeslider_swidth_]\n\
}\n\
\n\
$self formwindow\n\
\n\
}\n\
\n\
\n\
NamgraphView instproc formwindow {} {\n\
\n\
$self instvar title_ winname_ minx_ maxx_ miny_ maxy_ id_ modelobj_\n\
$self instvar slidetag_ timeslider\n\
\n\
toplevel $winname_\n\
\n\
set s $winname_\n\
wm title $s \"$title_-$id_\"\n\
\n\
set welcome [Animator set welcome]\n\
\n\
\n\
set width_ [NamgraphView set WinWidth]\n\
set height_ [NamgraphView set WinHeight]\n\
frame $s.menubar -bd 2 -width $width_ -relief raised\n\
\n\
menubutton $s.menubar.file -underline 0 -text \"File\" \\\n\
-menu $s.menubar.file.menu\n\
menu $s.menubar.file.menu\n\
$s.menubar.file.menu add command -label \"Save As ...\" \\\n\
-underline 0 -command \"puts OJ\" \\\n\
-state disabled\n\
\n\
\n\
$s.menubar.file.menu add command -label \"Close\" \\\n\
-underline 1 -command \"destroy $winname_\" \n\
\n\
menubutton $s.menubar.edit -underline 0 -text \"Edit\" \\\n\
-menu $s.menubar.edit.menu -state disabled\n\
menu $s.menubar.edit.menu\n\
$s.menubar.edit.menu add command -label \"Preferences\" \\\n\
-underline 0 -command \"puts OJ\" -state disabled\n\
\n\
\n\
menubutton $s.menubar.view -underline 0 -text \"View\" \\\n\
-menu $s.menubar.view.menu\n\
menu $s.menubar.view.menu\n\
\n\
foreach menuindex_ [$modelobj_ array names dataname] {\n\
set menuname_ [$modelobj_ set dataname($menuindex_)]\n\
$self tkvar $menuindex_\n\
set $menuindex_ 1\n\
$s.menubar.view.menu add checkbutton -label $menuname_ \\\n\
-variable [$self tkvarname $menuindex_] \\\n\
-command \"$self RefreshWin $menuindex_\"\n\
}\n\
\n\
pack $s.menubar.file -side left\n\
pack $s.menubar.edit -side left\n\
pack $s.menubar.view -side left\n\
\n\
\n\
\n\
bind $s <Control-w> \"destroy $s\"\n\
bind $s <Control-W> \"destroy $s\"\n\
bind $s <Control-q> \"[AnimControl instance] done\"\n\
bind $s <Control-Q> \"[AnimControl instance] done\"\n\
\n\
\n\
\n\
frame $s.main -bd 2\n\
canvas $s.main.c -width $width_ -height $height_ -background #ffffff\n\
\n\
\n\
$s.main.c bind clickable <Button-1> \"$self startclick \\%W \\%x \\%y\"\n\
$s.main.c bind clickable <Enter> \"$self showclick \\%W \\%x \\%y\"\n\
$s.main.c bind clickable <Leave> \"$self endshow \\%W \\%x \\%y\"\n\
$s.main.c bind clickable <ButtonRelease-1> \"$self endclick \\%W \\%x \\%y\"\n\
\n\
bind $s.main.c <Button-1> \"$self startrect \\%W \\%x \\%y\"\n\
bind $s.main.c <B1-Motion> \"$self dragrect \\%W \\%x \\%y\"\n\
bind $s.main.c <ButtonRelease-1> \"$self endrect \\%W \\%x \\%y\"\n\
\n\
canvas $s.main.c.cl -width 16 -height 400 -background #6b7787\n\
canvas $s.main.c.cr -width 16 -height 400 -background #6b7787\n\
pack $s.main.c.cl -side left -fill y\n\
pack $s.main.c.cr -side right -fill y\n\
pack $s.main.c -side left -fill both -expand yes\n\
$self build.c0 $s.main.c\n\
\n\
\n\
if { $slidetag_ == 1 } {\n\
frame $s.slider -bd 1 -relief flat\n\
set an_id [$modelobj_ set animator_id_]\n\
set mslider [$an_id getmainslidermodel]\n\
set vslider [new TimesliderNamgraphView $s.slider $mslider]\n\
$vslider setattachedView $self\n\
set timeslider(swidth) [$vslider set timeslider(swidth)]\n\
\n\
$modelobj_ set timeslider_swidth_ $timeslider(swidth)\n\
\n\
$mslider addObserver $vslider\n\
}\n\
\n\
\n\
frame $s.status -relief flat -borderwidth 1  -background #6b7787\n\
label $s.status.rem -text $welcome -textvariable \\\n\
remark -relief sunken\n\
pack $s.status.rem -side left -fill both -expand yes\n\
\n\
\n\
grid config $s.menubar -column 0 -row 0 \\\n\
-columnspan 1 -rowspan 1 -sticky \"snew\"\n\
grid config $s.main    -column 0 -row 1 \\\n\
-columnspan 1 -rowspan 1 -sticky \"snew\"\n\
\n\
if { $slidetag_ == 1 } {\n\
grid config $s.slider  -column 0 -row 2 \\\n\
-columnspan 1 -rowspan 1 -sticky \"snew\"\n\
\n\
grid config $s.status  -column 0 -row 3 \\\n\
-columnspan 1 -rowspan 1 -sticky \"snew\"\n\
\n\
} else {\n\
\n\
grid config $s.status  -column 0 -row 2 \\\n\
-columnspan 1 -rowspan 1 -sticky \"snew\"\n\
}\n\
\n\
grid columnconfigure $s 0 -weight 1\n\
grid rowconfigure $s 1 -weight 1\n\
\n\
}\n\
\n\
NamgraphView instproc RefreshWin { indexname } {\n\
\n\
\n\
$self instvar current_win_\n\
\n\
$self view_callback $current_win_\n\
\n\
\n\
}\n\
\n\
NamgraphView instproc build.c0 { w } {\n\
$self instvar current_win_ $w\n\
set current_win_ $w\n\
\n\
bind $w <Configure> \"$self view_callback $w\"\n\
}   \n\
\n\
\n\
NamgraphView instproc view_callback { w } {\n\
\n\
$self instvar modelobj_ slidetag_ \n\
$self instvar minx_ maxx_ miny_ maxy_\n\
\n\
$w delete all\n\
\n\
foreach menuindex_ [$modelobj_ array names dataname] {\n\
$self tkvar $menuindex_\n\
set menuvalue [set $menuindex_]\n\
if { $menuvalue == 1 } {\n\
set plotx [$modelobj_ set dataplotx($menuindex_)]\n\
set ploty [$modelobj_ set dataploty($menuindex_)]\n\
set plotcolor [$modelobj_ set plotcolor($menuindex_)]\n\
set plotmark [$modelobj_ set plotmark($menuindex_)]\n\
\n\
set cnt 0\n\
foreach xvalue $plotx {\n\
if { $xvalue > $minx_ && $xvalue < $maxx_ } {\n\
$self plot_xy $w $xvalue \\\n\
[lindex $ploty $cnt] $minx_ $maxx_ \\\n\
$miny_ $maxy_ $plotcolor \\\n\
$plotmark &slidetag_\n\
}\n\
incr cnt\n\
}\n\
}\n\
}\n\
\n\
$self draw_scale_xy 0\n\
}\n\
\n\
NamgraphView instproc plot_xy { c x y minx maxx miny maxy colorname markname traceflag} {\n\
$self instvar timeslider\n\
\n\
\n\
set scrxr [winfo width $c]\n\
set scryb [winfo height $c] \n\
\n\
set scrxr [expr $scrxr-$timeslider(swidth)]\n\
\n\
set yloc [expr $scryb-(($y-$miny)*$scryb/($maxy-$miny))]\n\
set xloc [expr $timeslider(swidth)/2 + (($x-$minx)*$scrxr/($maxx-$minx))]\n\
\n\
$c create bitmap $xloc $yloc -bitmap \"$markname\" -tag clickable \\\n\
-foreground $colorname\n\
\n\
}\n\
\n\
NamgraphView instproc draw_traceline { t } {\n\
$self instvar timeslider minx_ maxx_ current_win_ \n\
$self tkvar traceline\n\
\n\
set c $current_win_\n\
\n\
if { ($t < $minx_ ) || ($t > $maxx_) } {\n\
$c delete traceline\n\
return\n\
}\n\
\n\
set scrxl [expr $timeslider(swidth)/2]\n\
set scryt 0\n\
\n\
set scrxr [winfo width $c]\n\
set scryb [winfo height $c]\n\
set scrxr [expr $scrxr-$timeslider(swidth)]\n\
\n\
\n\
set traceline_x [expr $scrxl + ($t-$minx_)*$scrxr/($maxx_-$minx_)]\n\
$c delete traceline\n\
$c addtag traceline withtag \\\n\
[$c create line $traceline_x 0 $traceline_x $scryb -fill red]\n\
\n\
}\n\
\n\
NamgraphView instproc startclick {w x y} {\n\
$self instvar draglocation\n\
\n\
catch {unset draglocation}\n\
set draglocation(obj) [$w find closest $x $y]\n\
\n\
set draglocation(origx) $x\n\
set draglocation(origy) $y\n\
}   \n\
\n\
\n\
NamgraphView instproc endclick { w x y } {\n\
\n\
$self instvar draglocation timeslider maxx_ minx_ modelobj_\n\
\n\
if { $x == $draglocation(origx) && $y == $draglocation(origy)} {\n\
\n\
\n\
set width [winfo width $w]\n\
\n\
set width [expr $width-$timeslider(swidth)]\n\
set x [expr $x-$timeslider(swidth)/2]\n\
set t [expr $minx_+($maxx_-$minx_)*$x/$width]\n\
\n\
\n\
set animator [$modelobj_ animator]\n\
\n\
$animator settime $t\n\
}\n\
\n\
}\n\
\n\
NamgraphView instproc showclick { w x y } {\n\
\n\
set z [$self viewloc_proc $w $x $y]\n\
set show_msg \"Packet # [lindex $z 1] at time [lindex $z 0]\"\n\
\n\
catch {destroy $w.f}\n\
frame $w.f -relief groove -borderwidth 2\n\
message $w.f.msg -width 8c -text $show_msg\n\
pack $w.f.msg\n\
pack $w.f\n\
catch {place_frame $w $w.f [expr $x+10] [expr $y+10]}\n\
}\n\
\n\
NamgraphView instproc endshow { w x y } {\n\
catch {destroy $w.f}\n\
}\n\
\n\
\n\
NamgraphView instproc viewloc_proc  { w x y } {\n\
\n\
$self instvar timeslider \\\n\
minx_ maxx_ miny_ maxy_\n\
\n\
set width [winfo width $w]\n\
set height [winfo height $w]\n\
\n\
set width [expr $width-$timeslider(swidth)]\n\
set x [expr $x-$timeslider(swidth)/2]\n\
\n\
set x [expr $minx_+($maxx_-$minx_)*$x/$width]\n\
set y  [expr $miny_+1.0*($maxy_-$miny_)*($height-$y)/$height]\n\
\n\
set z [format \"%7.5f %10.0f\" $x $y ]\n\
return $z\n\
}\n\
\n\
#define zoom space \n\
\n\
NamgraphView instproc startrect {w x y} { \n\
$self instvar rectlocation\n\
\n\
catch {unset rectlocation} \n\
\n\
set rectlocation(xorig) $x\n\
set rectlocation(yorig) $y\n\
set tx [expr $x + 1] \n\
set ty [expr $y + 1]\n\
set rectlocation(obj) \\\n\
[$w create rect $x $y $tx $ty -outline gainsboro]\n\
}\n\
\n\
NamgraphView instproc dragrect {w x y} {\n\
$self instvar rectlocation\n\
$w delete $rectlocation(obj)\n\
set rectlocation(obj) \\\n\
[$w create rect $rectlocation(xorig) $rectlocation(yorig) \\\n\
$x $y -outline gainsboro]\n\
}   \n\
\n\
NamgraphView instproc endrect {w x y} {\n\
$self instvar timeslider title_ modelobj_ rectlocation\n\
$self instvar minx_ maxx_ miny_ maxy_\n\
\n\
$w delete $rectlocation(obj)\n\
\n\
if { $x == $rectlocation(xorig) || $y == $rectlocation(yorig)} {return}\n\
\n\
\n\
set width [winfo width $w]\n\
set height [winfo height $w]\n\
\n\
set width [expr $width-$timeslider(swidth)]\n\
set x [expr $x-$timeslider(swidth)/2]\n\
set rectlocation(xorig) [expr $rectlocation(xorig)-$timeslider(swidth)/2]\n\
\n\
\n\
set timestart [expr $minx_+($maxx_-$minx_)*$rectlocation(xorig)/$width]\n\
set timeend   [expr $minx_+($maxx_-$minx_)*$x/$width]\n\
set seqstart  [expr $miny_+1.0*($maxy_-$miny_)* \\\n\
($height-$rectlocation(yorig))/$height]\n\
set seqend    [expr $miny_+1.0*($maxy_-$miny_)*($height-$y)/$height]\n\
\n\
if { $timestart > $timeend } {\n\
set tmpt $timestart\n\
set timestart $timeend\n\
set timeend $tmpt\n\
}\n\
\n\
if { $seqstart > $seqend } {\n\
set tmpt $seqstart\n\
set seqstart $seqend\n\
set seqend $tmpt\n\
}\n\
\n\
set zoomview [new NamgraphView $title_ $modelobj_ $timestart \\\n\
$timeend $seqstart $seqend \"ZOOM\"]\n\
$modelobj_ addObserver $zoomview\n\
\n\
}\n\
\n\
NamgraphView instproc startmove {w o x y} {\n\
$self instvar rectlocation\n\
scan [$w coords $o] \"%f %f %f %f\" x1 y1 x2 y2\n\
set rectlocation(x) [expr abs($x - $x1)]\n\
set rectlocation(y) [expr abs($y - $y1)]\n\
}\n\
\n\
NamgraphView instproc moverect {w o x y} {\n\
$self instvar rectlocation\n\
scan [$w coords $o] \"%f %f %f %f\" x1 y1 x2 y2\n\
set dx [expr $x - $x1 - $rectlocation(x)]\n\
set dy [expr $y - $y1 - $rectlocation(y)]\n\
$w move $o $dx $dy\n\
}\n\
\n\
NamgraphView instproc draw_scale_xy { mode } {\n\
\n\
$self instvar timeslider minx_ maxx_ miny_ maxy_ current_win_\n\
$self instvar modelobj_\n\
\n\
set c $current_win_\n\
\n\
set scrxl [expr $timeslider(swidth)/2]\n\
set scryt 0\n\
\n\
set scrxr [winfo width $c]\n\
set scryb [winfo height $c]\n\
\n\
set scrxr [expr $scrxr-$timeslider(swidth)]\n\
\n\
set nxpoints 8\n\
set nypoints 6\n\
\n\
set yincr [expr ($maxy_-$miny_)/$nypoints.0]\n\
set xincr [expr ($maxx_-$minx_)/$nxpoints.0]\n\
\n\
for {set i 1} {$i<$nypoints} {incr i} {\n\
set yval [expr $miny_+$i*$yincr]\n\
set yval [expr int($yval)]\n\
set ypos [expr $scryb-($scryb*($yval-$miny_)/($maxy_-$miny_))]\n\
set yval [$modelobj_ verifyymark $yval]\n\
\n\
$c create line $scrxl $ypos [expr $scrxl+10] $ypos -fill #6725a0\n\
$c create text [expr $scrxl+15] [expr $ypos-7] -text $yval -anchor nw \\\n\
-justify left -fill #6725a0 \\\n\
-font \"-adobe-new century schoolbook-medium-r-normal--10-100-75-75-p-60-iso8859-1\"\n\
}\n\
\n\
for {set i 1} {$i<$nxpoints} {incr i} {\n\
set xpos [expr ($scrxr/$nxpoints.0)*$i + $scrxl]\n\
set xval [format \"%+7.5f\" [expr $minx_+($i*$xincr)]]\n\
$c create line $xpos 0 $xpos 10 -fill #6725a0\n\
$c create text  $xpos 15 -text $xval -anchor n\\\n\
-justify left -fill #6725a0 \\\n\
-font \"-adobe-new century schoolbook-medium-r-normal--10-100-75-75-p-60-iso8859-1\"\n\
}\n\
}\n\
\n\
NamgraphView instproc update { time } {\n\
\n\
$self instvar now_ winname_ modelobj_\n\
\n\
if ![winfo exists $winname_ ] {\n\
$modelobj_ deleteObserver $self\n\
$self destroy\n\
return\n\
}\n\
\n\
if { [string compare $time \"*\"] != 0 } {\n\
set now_ $time\n\
$self draw_traceline $now_\n\
}\n\
}\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
Class NamgraphModel -superclass Observable\n\
\n\
NamgraphModel instproc init { nid obs } {\n\
\n\
$self next\n\
\n\
$self instvar id_ datacount Maxy_ Maxx_ animator_id_ timeslider_swidth_\n\
set id_ $nid\n\
set datacount 0\n\
set animator_id_ $obs\n\
\n\
set Maxy_ [$obs set highest_seq($id_)]\n\
set Maxx_ [$obs set maxtime]\n\
\n\
}\n\
\n\
NamgraphModel instproc id {} {\n\
$self instvar id_\n\
\n\
return $id_\n\
\n\
}\n\
\n\
NamgraphModel instproc animator {} {\n\
\n\
$self instvar animator_id_\n\
return $animator_id_\n\
\n\
}\n\
\n\
NamgraphModel instproc adddata { ob mid x y ymark } {\n\
\n\
$self instvar dataname datacount id_ dataplotx dataploty\n\
$self instvar plotmark plotcolor dataplotym\n\
\n\
$self tkvar midbuilder\n\
\n\
if ![info exists midbuilder($mid)] {\n\
set midbuilder($mid) $datacount\n\
\n\
set mname [$ob set filter_id($mid)]\n\
set pmark [$ob set plotmark($id_.$mid)]\n\
set pcolor [$ob set plotcolor($id_.$mid)]\n\
set pcolor [$ob set colorname($pcolor)]\n\
\n\
set dataname($datacount) $mname \n\
set plotmark($datacount) $pmark\n\
set plotcolor($datacount) $pcolor\n\
incr datacount\n\
}\n\
\n\
set current_index $midbuilder($mid)\n\
\n\
lappend dataplotx($current_index) $x\n\
lappend dataploty($current_index) $y\n\
set dataplotym($y) $ymark\n\
\n\
}\n\
\n\
NamgraphModel instproc verifyymark { index } {\n\
\n\
$self instvar dataplotym\n\
\n\
if [info exists dataplotym($index)] {\n\
\n\
return $dataplotym($index)\n\
\n\
}\n\
\n\
return \"\"\n\
\n\
}\n\
\n\
NamgraphModel instproc startview {title} {\n\
\n\
$self instvar Maxy_ Maxx_\n\
\n\
set vob [new NamgraphView $title $self 0 $Maxx_ 0 $Maxy_ 1]\n\
$self addObserver $vob\n\
}\n\
\n\
NamgraphModel instproc update { arg } {\n\
\n\
set now [get_trace_item \"-t\" $arg]\n\
if { $now == \"\" } {\n\
set now $arg\n\
$self notifyObservers $now \n\
} else {\n\
}\n\
}\n\
Class TimesliderModel -superclass Observable\n\
\n\
TimesliderModel instproc init { min_t max_t curr_t aid} {\n\
$self next\n\
\n\
$self instvar mint_ maxt_ currt_ aid_\n\
\n\
set mint_ $min_t\n\
set maxt_ $max_t\n\
set currt_ $curr_t\n\
set aid_ $aid\n\
\n\
}\n\
\n\
TimesliderModel instproc setmintime {min_t} {\n\
$self instvar mint_\n\
set mint_ $min_t\n\
set e [list $mint_ min]\n\
$self notifyObservers $e\n\
}\n\
\n\
TimesliderModel instproc setmaxtime {max_t} {\n\
$self instvar maxt_\n\
set maxt_ $max_t\n\
set e [list $maxt_ max]\n\
$self notifyObservers $e\n\
}\n\
\n\
TimesliderModel instproc setcurrenttime {curr_t} {\n\
$self instvar currt_\n\
set currt_ $curr_t\n\
set e [list $curr_t now]\n\
$self notifyObservers $e\n\
}\n\
\n\
TimesliderModel instproc getmintime {} {\n\
$self instvar mint_\n\
return $mint_\n\
}\n\
\n\
TimesliderModel instproc getmaxtime {} {\n\
$self instvar maxt_\n\
return $maxt_\n\
}\n\
\n\
TimesliderModel instproc getcurrenttime {} {\n\
$self instvar currt_\n\
return $currt_\n\
}   \n\
\n\
TimesliderModel instproc getanimator {} {\n\
$self instvar aid_\n\
return $aid_\n\
}  \n\
\n\
TimesliderModel instproc setpipemode { p } {\n\
$self instvar pipemode_\n\
set pipemode_ $p\n\
}  \n\
\n\
TimesliderModel instproc getpipemode { } {\n\
$self instvar pipemode_\n\
return $pipemode_\n\
}  \n\
\n\
\n\
Class TimesliderView -superclass Observer\n\
\n\
TimesliderView instproc frame {} {\n\
$self instvar timeslider\n\
return $timeslider(frame)\n\
}\n\
\n\
TimesliderView instproc swidth {} {\n\
$self instvar timeslider\n\
return $timeslider(swidth)\n\
}\n\
\n\
TimesliderView instproc init {w mid} {\n\
$self next\n\
\n\
$self instvar timeslider timeslider_width timeslider_tag \\\n\
timeslider_pos range mid_ window_frame_ \\\n\
marker_id_counter_ delete_marker_\n\
\n\
set mid_ $mid\n\
\n\
set mintime [$mid_ getmintime]\n\
set maxtime [$mid_ getmaxtime]\n\
\n\
set range [expr $maxtime - $mintime]\n\
\n\
set marker_id_counter_ 0\n\
set delete_marker_ 0\n\
\n\
set window_frame_ $w.f\n\
frame $w.f -borderwidth 2 -relief groove\n\
pack $w.f -side top -fill x -expand 1 \n\
\n\
set timeslider(height) 24\n\
set timeslider(swidth) 32\n\
set timeslider(width) 32 \n\
set timeslider_width($w.f.c) 32\n\
set timeslider(canvas) $w.f.c\n\
set timeslider(frame) $w \n\
\n\
canvas $w.f.c -height $timeslider(height) -width 300 \\\n\
-background white -highlightthickness 0\n\
pack $w.f.c -side left -fill x -expand 1 -padx 0 -pady 0\n\
\n\
label $w.f.c.b -bitmap time -highlightthickness 0 -borderwidth 1 \\\n\
-relief raised\n\
set timeslider_tag($w.f.c) [$w.f.c create window \\\n\
[expr $timeslider(swidth)/2] 0 -window $w.f.c.b \\\n\
-height 12 -width $timeslider(swidth) -anchor n]\n\
set timeslider_pos($w.f.c) 0\n\
\n\
bind $w.f.c <ButtonPress-1> \"$self timeslidertrough $w.f.c %x %y\"\n\
bind $w.f.c.b <ButtonPress-1> \"$self timesliderpress $w.f.c %x %y;break\"\n\
bind $w.f.c.b <ButtonRelease-1> \"$self timesliderrelease $w.f.c %x %y\"\n\
bind $w.f.c.b <B1-Motion> \"$self timeslidermotion $w.f.c %x %y\"\n\
bind $w.f.c <Configure> \"$self timeticks $w.f.c\"\n\
}\n\
\n\
TimesliderView instproc destroy {} {\n\
$self instvar window_frame_\n\
\n\
if {[winfo exists $window_frame_]} {\n\
foreach widget [pack slaves $window_frame_] {\n\
set subwidgets [pack slaves $widget]\n\
if {[llength $subwidgets] > 0} {\n\
pack forget $subwidgets\n\
destroy $subwidgets\n\
}\n\
pack forget $widget\n\
destroy $widget\n\
}\n\
}\n\
catch {destroy $window_frame_}\n\
\n\
$self next\n\
}\n\
\n\
TimesliderView instproc timeticks { wfc } {\n\
$self instvar timeslider range timeslider_width \\\n\
timeslider_tag mid_ \n\
\n\
set timeslider(canvas) $wfc\n\
\n\
set width [winfo width $timeslider(canvas)]\n\
\n\
set mintime [$mid_ getmintime]\n\
set maxtime [$mid_ getmaxtime]\n\
\n\
set timeslider(width) $width\n\
set timeslider_width($wfc) $width\n\
$timeslider(canvas) delete ticks\n\
\n\
pack forget $timeslider(canvas)\n\
update\n\
update idletasks\n\
pack $timeslider(canvas) -side left -fill x -expand 1 -padx 0 -pady 0\n\
\n\
set width [winfo width $wfc]\n\
\n\
set x [expr $timeslider(swidth)/2]\n\
\n\
set tickIncr [expr $range / ($width-$timeslider(swidth))]\n\
\n\
if {$range == 0} {\n\
set intertick [expr ($width - $timeslider(swidth)) / 10]\n\
} else {\n\
set intertick [expr ($width-$timeslider(swidth))/(10 * $range)]\n\
}\n\
\n\
if {$intertick < 2} {\n\
set intertick 2\n\
}\n\
\n\
for {set t $mintime} {$t < ($range+$mintime)} {set t [expr $t+$intertick*$tickIncr]} {\n\
set intx [expr int($x)]\n\
$timeslider(canvas) addtag ticks withtag \\\n\
[$timeslider(canvas) create line \\\n\
$intx [expr $timeslider(height)/2] \\\n\
$intx $timeslider(height)]\n\
set x [expr $x+$intertick]\n\
}\n\
\n\
set x [expr $timeslider(swidth)/2]\n\
if {$range == 0} {\n\
set intertick [expr $width - $timeslider(swidth)]\n\
} else {\n\
set intertick [expr ($width-$timeslider(swidth))/($range)]\n\
}\n\
if {$intertick < 20} {\n\
set intertick 20\n\
}\n\
\n\
for {set t $mintime} {$t < ($range+$mintime)} {set t [expr $t+$intertick*$tickIncr]} {\n\
set intx [expr int($x)]\n\
$timeslider(canvas) addtag ticks withtag \\\n\
[$timeslider(canvas) create line \\\n\
$intx 0 $intx $timeslider(height) -fill blue]\n\
set x [expr $x+$intertick]\n\
}\n\
\n\
$timeslider(canvas) delete arrow\n\
set pmode [$mid_ getpipemode]\n\
if {$pmode == 1 } {\n\
set x [expr $width-$timeslider(swidth)/2]\n\
set y 0\n\
\n\
$timeslider(canvas) addtag arrow withtag \\\n\
[$timeslider(canvas) create polygon $x $y \\\n\
[expr $x+$timeslider(swidth)/2] [expr $y+$timeslider(height)/2] \\\n\
$x [expr $y+$timeslider(height)]]\n\
}\n\
\n\
set wfc_width [winfo width $wfc]\n\
set now [$mid_ getcurrenttime]\n\
if {$maxtime > 0} {\n\
set x [expr ($wfc_width-$timeslider(swidth))*$now/$maxtime]\n\
} else {\n\
set x [expr ($wfc_width-$timeslider(swidth))*$now]\n\
}\n\
$wfc coords $timeslider_tag($wfc) [expr $x + $timeslider(swidth)/2] 0\n\
}\n\
\n\
\n\
TimesliderView instproc timesliderpress {w x y} {\n\
$self instvar timeslider mid_\n\
\n\
set timeslider(oldpos) $x\n\
$w.b configure -relief sunken\n\
\n\
set animator [$mid_ getanimator]\n\
\n\
$animator setsliderPressed 1\n\
}\n\
\n\
\n\
TimesliderView instproc timeslidertrough {w x y} {\n\
$self instvar timeslider sliderPressed timeslider_pos mid_\n\
set animator [$mid_ getanimator]\n\
\n\
if {$timeslider_pos($w)>$x} {\n\
$animator rewind\n\
} else {\n\
$animator fast_fwd\n\
}\n\
}\n\
\n\
TimesliderView instproc timeslidermotion {wc x y} {\n\
$self instvar timeslider range timeslider_tag \\\n\
timeslider_pos timeslider_width mid_\n\
\n\
set mintime [$mid_ getmintime]\n\
\n\
set diff [expr $x - $timeslider(oldpos)]\n\
\n\
set timeslider(canvas) $wc\n\
\n\
$timeslider(canvas) move $timeslider_tag($wc) \\\n\
$diff 0\n\
\n\
set timeslider_pos($wc) [expr $timeslider_pos($wc) + $diff]\n\
if {$timeslider_pos($wc)<0} {\n\
$timeslider(canvas) move $timeslider_tag($wc) \\\n\
[expr 0 - $timeslider_pos($wc)] 0\n\
set timeslider_pos($wc) 0\n\
}\n\
if {$timeslider_pos($wc)>[expr $timeslider_width($wc)-$timeslider(swidth)]} {\n\
$timeslider(canvas) move $timeslider_tag($wc) \\\n\
[expr ($timeslider_width($wc)-$timeslider(swidth))-$timeslider_pos($wc)] 0\n\
set timeslider_pos($wc) [expr $timeslider_width($wc)-$timeslider(swidth)]\n\
}\n\
set tick 0\n\
catch {\n\
set tick [expr 1000.0*$timeslider_pos($wc)/($timeslider_width($wc)-$timeslider(swidth))]\n\
}\n\
set now [expr ($tick * $range) / 1000. + $mintime]\n\
set timeslider(tick) $tick\n\
\n\
set animator [$mid_ getanimator]\n\
$animator setCurrentTime [format %.6f $now]\n\
\n\
$self timesliderset $now\n\
}\n\
\n\
TimesliderView instproc timesliderrelease {w x y} {\n\
$self instvar timeslider running mid_\n\
set animator [$mid_ getanimator]\n\
\n\
$self timeslidermotion $w $x $y\n\
$w.b configure -relief raised\n\
$animator slidetime $timeslider(tick) 1\n\
$animator setsliderPressed 0\n\
\n\
if [$animator getrunning] {\n\
$animator renderFrame\n\
}\n\
}\n\
\n\
TimesliderView instproc update { timeset } {\n\
$self instvar timeslider range mid_\n\
\n\
set time [lindex $timeset 0]\n\
set type [lindex $timeset 1]\n\
\n\
if {[string compare $type \"now\"] == 0} {\n\
$self timesliderset $time \n\
}\n\
\n\
if {[string compare $type \"min\"] == 0} {\n\
$self timesliderset $time \n\
}\n\
\n\
if {[string compare $type \"max\"] == 0} {\n\
set mintime [$mid_ getmintime]\n\
set range [expr $time - $mintime]\n\
$self timeticks $timeslider(canvas) \n\
}\n\
\n\
}\n\
\n\
TimesliderView instproc timesliderset {time} {\n\
$self instvar timeslider timeslider_width timeslider_tag \\\n\
timeslider_pos mid_ range\n\
\n\
set minimum_time [$mid_ getmintime]\n\
\n\
if {[winfo exists $timeslider(canvas)] == 1} {\n\
set x [expr ($timeslider_width($timeslider(canvas)) - $timeslider(swidth)) * \\\n\
($time - $minimum_time)]\n\
\n\
if {$range > 0} {\n\
set x [expr $x / $range]\n\
} else {\n\
}\n\
\n\
$timeslider(canvas) coords \\\n\
$timeslider_tag($timeslider(canvas)) \\\n\
[expr $x + $timeslider(swidth)/2] 0\n\
\n\
set timeslider_pos($timeslider(canvas)) $x\n\
}\n\
}\n\
\n\
\n\
TimesliderView instproc addMarker {time label_text color} {\n\
$self instvar timeslider timeslider_width range mid_ \\\n\
marker_id_counter_ markers_\n\
\n\
set mintime [$mid_ getmintime]\n\
\n\
set width [winfo width $timeslider(canvas)]\n\
\n\
set x [expr $timeslider(swidth)/2]\n\
\n\
set tickIncr [expr $range / ($width - $timeslider(swidth))]\n\
\n\
if {$range == 0} {\n\
set intertick [expr ($width - $timeslider(swidth))]\n\
} else {\n\
set intertick [expr ($width - $timeslider(swidth))/$range]\n\
}\n\
\n\
if {$intertick < 2} {\n\
set intertick 2\n\
}\n\
\n\
\n\
set x [expr ($timeslider(swidth)/2) + ($intertick * ($time - $mintime))]\n\
\n\
set marker $timeslider(canvas).marker$marker_id_counter_\n\
set marker_id_counter_ [expr $marker_id_counter_ + 1]\n\
\n\
label $marker -text $label_text -relief raised -background $color\n\
\n\
$timeslider(canvas) create window $x [expr $timeslider(height)/2] \\\n\
-window $marker \\\n\
-height [expr $timeslider(height)/2] -width 15 \\\n\
-anchor n -tags markers\n\
\n\
set markers_($marker) \"$time $label_text\"\n\
\n\
bind $marker <ButtonPress-1> \"$marker configure -relief sunken\"\n\
bind $marker <ButtonRelease-1> \"$marker configure -relief raised; \\\n\
$self handleMarkerClick $marker\"\n\
bind $marker <B1-Motion> \n\
}\n\
\n\
\n\
TimesliderView instproc clearMarkers {} {\n\
$self instvar timeslider marker_id_counter_\n\
\n\
for {set id 0} {$id < $marker_id_counter_} {set id [expr $id + 1]} {\n\
destroy $timeslider(canvas).marker$id\n\
}\n\
\n\
$timeslider(canvas) delete markers\n\
set marker_id_counter_ 0\n\
}\n\
\n\
\n\
TimesliderView instproc handleMarkerClick {marker} {\n\
$self instvar delete_marker_ mid_ markers_\n\
\n\
set animator [$mid_ getanimator]\n\
set time [lindex $markers_($marker) 0]\n\
set label_text [lindex $markers_($marker) 1]\n\
\n\
if {$delete_marker_ != 1} {\n\
$animator moveToMarkerTime $time\n\
} else {\n\
destroy $marker\n\
$animator removeMarker $time $label_text\n\
}\n\
}\n\
\n\
Class TimesliderNamgraphView -superclass TimesliderView\n\
\n\
TimesliderNamgraphView instproc setattachedView {viewid} {\n\
$self instvar _attachedviewid\n\
\n\
set _attachedviewid $viewid\n\
\n\
}\n\
\n\
TimesliderNamgraphView instproc update {timeset} {\n\
\n\
$self next $timeset\n\
\n\
set time [lindex $timeset 0]\n\
set type [lindex $timeset 1]    \n\
\n\
if {[string compare $type \"max\"] == 0} {\n\
\n\
$self instvar _attachedviewid\n\
set s [$_attachedviewid set current_win_]\n\
$_attachedviewid set maxx_ $time\n\
$_attachedviewid view_callback $s\n\
\n\
}\n\
\n\
}\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
NetworkModel/Editor set maximum_simulation_time_ 60.0\n\
\n\
Class Editor \n\
\n\
Editor set uniqueID_ 0\n\
\n\
Editor proc getid {} {\n\
set id [Editor set uniqueID_]\n\
Editor set uniqueID_ [expr $id + 1]\n\
return $id\n\
}\n\
\n\
Editor instproc init {tracefile} {\n\
$self instvar menulist_ SharedEnv id_ editorwindow_ \\\n\
editor_view_ balloon_ menubar_ save_filename_ \\\n\
agent_types_ source_types_ lossmodel_types_ \\\n\
toolbar_buttons_ current_tool_ netmodel_ mintime_ \\\n\
now_ slider_model_ slider_view_ pipemode_ current_time_ \\\n\
simulatior_property_window_\n\
\n\
$self tkvar current_agent_ current_time_ current_source_ current_lossmodel_\n\
\n\
set current_time_ 0.0\n\
\n\
set id_ [Editor getid]\n\
toplevel .nameditor-$id_\n\
set editorwindow_ .nameditor-$id_\n\
if {$tracefile == \"\"} {\n\
wm title $editorwindow_ \"NAM Editor - $id_\"\n\
} else {\n\
wm title $editorwindow_ \"NAM Editor - $tracefile\"\n\
}\n\
\n\
set agent_types_ {TCP TCP/Reno \\\n\
TCP/Vegas TCP/Sack1 TCP/Fack UDP - Null TCPSink \\\n\
TCPSink/DelAck TCPSink/Sack1}\n\
set current_agent_ UDP\n\
\n\
set source_types_ {CBR Exponential FTP Pareto Telnet} \n\
set current_source_ CBR\n\
\n\
set lossmodel_types_ {Periodic Uniform}\n\
set current_lossmodel_ Periodic\n\
\n\
set SharedEnv(CurrentPageId) 1\n\
set SharedEnv(CurrentCanvas) \"\" \n\
set SharedEnv(PageWidth) 8.5i\n\
set SharedEnv(PageHeight) 11i\n\
set SharedEnv(PageVisX) 3i\n\
set SharedEnv(PageVisY) 2i \n\
set SharedEnv(SlideBG) white\n\
set SharedEnv(Landscape) 0 \n\
set SharedEnv(ColorWidth) 400      \n\
set SharedEnv(ColorHeight) 100\n\
set SharedEnv(PrivateCmap) \"\"\n\
set SharedEnv(ButtonOrient) left\n\
set SharedEnv(ButtonCount)  10\n\
set SharedEnv(ButtonSize) .35i\n\
set SharedEnv(ButtonRelief) flat\n\
set SharedEnv(ButtonStipple1) gray50\n\
set SharedEnv(ButtonStipple2) {}\n\
set SharedEnv(ButtonColor) cyan\n\
set SharedEnv(Fill) \"\" \n\
set SharedEnv(MenuSelect) 0\n\
\n\
button .b\n\
set SharedEnv(Background) [.b cget -bg]\n\
set SharedEnv(ActiveBackground) [.b cget -activebackground]\n\
destroy .b\n\
\n\
set SharedEnv(DefButtonColor) [. cget -bg]\n\
set SharedEnv(Cut) \"\"\n\
set SharedEnv(Copy) \"\"\n\
set SharedEnv(Message) \"Select\"\n\
set SharedEnv(Xpos) 0\n\
set SharedEnv(Ypos) 0\n\
set SharedEnv(Smooth) 0\n\
set SharedEnv(Outline) black\n\
set SharedEnv(Stipple) \"\"\n\
set SharedEnv(Arrow) none\n\
set SharedEnv(JoinStyle) miter \n\
set SharedEnv(Grid) 0\n\
set SharedEnv(GridX) .25i\n\
set SharedEnv(GridY) .25i\n\
set SharedEnv(UnitX) i\n\
set SharedEnv(UnitY) i\n\
set SharedEnv(ScreenH) [winfo screenheight .]\n\
set SharedEnv(ScreenW) [winfo screenwidth .]\n\
set SharedEnv(Gravity) 1\n\
set SharedEnv(GravityVal) 30\n\
set SharedEnv(Sub-Cursor) \"\"\n\
\n\
set SharedEnv(Start) \"\"\n\
set SharedEnv(top_left_corner) bottom_right_corner\n\
set SharedEnv(top_side) bottom_side\n\
set SharedEnv(top_right_corner) bottom_left_corner\n\
set SharedEnv(right_side) left_side\n\
set SharedEnv(bottom_right_corner) top_left_corner\n\
set SharedEnv(bottom_side) top_side\n\
\n\
set SharedEnv(bottom_left_corner) top_right_corner\n\
set SharedEnv(left_side) right_side\n\
set SharedEnv(FixedAspect) True\n\
\n\
set balloon_ [new BalloonHelp $editorwindow_]\n\
\n\
if {$tracefile == \"\"} {\n\
set save_filename_ \"\"\n\
} else {\n\
set save_filename_ $tracefile\n\
}\n\
\n\
set netmodel_ [new NetworkModel/Editor $self \" \"]\n\
$netmodel_ set Wpxmin_ 500.0\n\
$netmodel_ set Wpymin_ 500.0\n\
$netmodel_ set Wpxmax_ 625.0\n\
$netmodel_ set Wpymax_ 625.0\n\
\n\
\n\
\n\
frame $editorwindow_.menubar_ -relief raised -bd 2\n\
$self buildMenubar $editorwindow_.menubar_\n\
pack $editorwindow_.menubar_ -side top -fill x\n\
\n\
frame $editorwindow_.view\n\
$self buildEditorView $editorwindow_.view\n\
\n\
frame $editorwindow_.timecontrols -borderwidth 2 -relief groove\n\
$self buildTimeControls $editorwindow_.timecontrols\n\
\n\
frame $editorwindow_.tools -borderwidth 2 -relief groove\n\
$self buildToolbar $editor_view_ $editorwindow_.tools\n\
\n\
frame $editorwindow_.timeslider -relief flat -borderwidth 0\n\
$self buildTimeSlider $editorwindow_.timeslider\n\
\n\
\n\
pack $editorwindow_.tools $editorwindow_.timecontrols -side top -fill x\n\
pack $editorwindow_.timeslider -side bottom -fill x \n\
pack $editorwindow_.view -side left -fill both -expand true\n\
\n\
\n\
bind $editorwindow_ <q> \"destroy $editorwindow_\"\n\
bind $editorwindow_ <Q> \"destroy $editorwindow_\"\n\
bind $editorwindow_ <Control-w> \"destroy $editorwindow_\"\n\
bind $editorwindow_ <Control-W> \"destroy $editorwindow_\"\n\
bind $editorwindow_ <Control-q> \"[AnimControl instance] done\"\n\
bind $editorwindow_ <Control-Q> \"[AnimControl instance] done\"\n\
\n\
$self setCursor Select\n\
\n\
$self chooseAgent TCP \n\
$self chooseTrafficSource FTP\n\
$self chooseTool select\n\
\n\
\n\
if {$tracefile != \"\"} {\n\
$self loadNsScript $tracefile\n\
}\n\
}\n\
\n\
Editor instproc destroy {} {\n\
$self instvar balloon_\n\
delete $balloon_\n\
delete $slider_model_\n\
delete $slider_view_\n\
}\n\
\n\
Editor instproc done {} {\n\
$self instvar editorwindow_\n\
destroy $editorwindow_\n\
delete $self\n\
}\n\
\n\
Editor instproc buildEditorView {w} {\n\
$self instvar id_ SharedEnv netmodel_ \\\n\
editor_view_ editorwindow_ balloon_ magnification viewOffset\n\
\n\
\n\
frame $w.view\n\
frame $w.view.sunk -borderwidth 2 -relief sunken\n\
\n\
$netmodel_ view $w.view.sunk.net\n\
set editor_view_ $w.view.sunk.net\n\
set SharedEnv(CurrentCanvas) $editor_view_\n\
pack $w.view.sunk.net -side top -expand true -fill both\n\
\n\
$editor_view_ xscroll $w.view.hsb\n\
scrollbar $w.view.hsb -orient horizontal -width 10 -borderwidth 1 \\\n\
-command \"$editor_view_ xview\"\n\
$w.view.hsb set 0.0 1.0\n\
pack $w.view.hsb -side bottom -fill x\n\
pack $w.view.sunk -side top -fill both -expand true \n\
\n\
frame $w.yscroll\n\
$editor_view_ yscroll $w.yscroll.vsb\n\
scrollbar $w.yscroll.vsb -orient vertical -width 10 -borderwidth 1 \\\n\
-command \"$editor_view_ yview\"\n\
$w.yscroll.vsb set 0.0 1.0\n\
pack $w.yscroll.vsb -side top -fill y -expand true\n\
\n\
frame $w.yscroll.l -width 12 -height -12\n\
pack $w.yscroll.l -side top -ipady 6 \n\
\n\
set view $editor_view_\n\
\n\
set magnification 1.0\n\
set viewOffset(x) 0.0\n\
set viewOffset(y) 0.0\n\
\n\
frame $w.zoom -borderwidth 2 -relief groove\n\
frame $w.zoom.bar \n\
pack $w.zoom.bar -side top\n\
\n\
button $w.zoom.bar.zoomin -bitmap \"zoomin\" \\\n\
-command \"$view zoom 1.6\" \\\n\
-highlightthickness 0 \\\n\
-borderwidth 1\n\
\n\
button $w.zoom.bar.zoomout -bitmap \"zoomout\" \\\n\
-command \"$view zoom 0.625\" \\\n\
-highlightthickness 0 \\\n\
-borderwidth 1\n\
pack $w.zoom.bar.zoomin \\\n\
$w.zoom.bar.zoomout \\\n\
-side top\n\
\n\
$balloon_ balloon_for $w.zoom.bar.zoomin \"Zoom In\"\n\
$balloon_ balloon_for $w.zoom.bar.zoomout \"Zoom Out\"\n\
\n\
\n\
pack $w.zoom -side left -fill y\n\
pack $w.yscroll -side right -fill y\n\
pack $w.view -side left -fill both -expand true\n\
\n\
$self viewBind $editor_view_\n\
}\n\
\n\
Editor instproc viewBind {netView} {\n\
$self instvar SharedEnv \n\
\n\
bind $netView <Button-1>        \"$self handleLeftButtonClick \\%x \\%y\"\n\
bind $netView <Button-3>        \"$self handleRightButtonClick \\%x \\%y\"\n\
bind $netView <Any-B1-Motion>   \"$self handleLeftButtonDrag \\%x \\%y\"\n\
bind $netView <ButtonRelease-1> \"$self handleLeftButtonRelease \\%x \\%y\"\n\
bind $netView <Motion>          \"$self handleMouseMovement \\%x \\%y\"\n\
bind $netView <Double-Button-1> \"$self handleLeftButtonDoubleClick \\%x \\%y\"\n\
\n\
}\n\
\n\
Editor instproc handleRightButtonClick {x y} {\n\
$self instvar current_tool_\n\
\n\
set old_tool $current_tool_\n\
$self chooseTool select \n\
$self handleLeftButtonDoubleClick $x $y\n\
$self chooseTool $old_tool\n\
}\n\
\n\
Editor instproc handleLeftButtonDoubleClick {x y} {\n\
$self instvar SharedEnv editor_view_ current_tool_ editorwindow_\n\
\n\
switch $current_tool_ {\n\
select {\n\
set objinfo [$editor_view_ getObjectProperty $x $y]\n\
if {[string compare $objinfo \"NONE\"] == 0 } {\n\
return \n\
}\n\
\n\
set item [lindex $objinfo 0]\n\
set item [lindex $item 0]\n\
$self displayObjectProperties $editorwindow_.properties $item $x $y $objinfo\n\
place_frame $editorwindow_ $editorwindow_.properties $x $y\n\
}\n\
}\n\
}\n\
\n\
Editor instproc handleMouseMovement {x y} {\n\
$self instvar SharedEnv editor_view_ current_tool_\n\
\n\
set objinfo [$editor_view_ getObjectInformation $x $y]\n\
if {[string compare $objinfo \"NONE\"] != 0} {\n\
$editor_view_ showAgentLink $x $y\n\
} else {\n\
$editor_view_ hideAgentLinks\n\
}\n\
\n\
switch $current_tool_ {\n\
deleteobject -\n\
select {\n\
if {[string compare $objinfo \"NONE\"] != 0} {\n\
$self enterObject $editor_view_ $x $y $objinfo\n\
} else {\n\
catch {destroy $editor_view_.f}\n\
}\n\
}\n\
}\n\
}\n\
\n\
Editor instproc handleLeftButtonClick {x y} {\n\
$self instvar SharedEnv editor_view_ current_tool_\n\
$self tkvar current_agent_ current_source_ current_lossmodel_\n\
\n\
switch $current_tool_ {\n\
select      {$editor_view_ setPoint $x $y 0}\n\
addnode     {$editor_view_ addNode $x $y}\n\
addlink     {$editor_view_ addLink $x $y}\n\
addagent    {$editor_view_ addAgent $x $y $current_agent_}\n\
addtrafficsource {$editor_view_ addTrafficSource $x $y $current_source_}\n\
addlossmodel {$editor_view_ addLossModel $x $y $current_lossmodel_}\n\
deleteobject {\n\
$editor_view_ deleteObject $x $y \n\
}\n\
}\n\
$self renderFrame\n\
}\n\
\n\
Editor instproc handleLeftButtonRelease {x y} {\n\
$self instvar SharedEnv editor_view_ current_tool_ slider_view_\n\
\n\
switch $current_tool_ {\n\
select {\n\
set marker_list [$editor_view_ releasePoint $x $y]\n\
$slider_view_ clearMarkers\n\
if {$marker_list != \"NONE\"} {\n\
$self addTimeSliderNodeMarkers $marker_list\n\
}\n\
\n\
}\n\
addlink   {\n\
$editor_view_ releasePoint $x $y\n\
}\n\
}\n\
}\n\
\n\
Editor instproc handleLeftButtonDrag {x y} {\n\
$self instvar SharedEnv editor_view_ current_tool_\n\
\n\
switch $current_tool_ {\n\
select {\n\
$editor_view_ moveTo $x $y\n\
}\n\
addlink   {\n\
$editor_view_ moveTo $x $y \n\
}\n\
}\n\
}\n\
\n\
\n\
Editor instproc showPalette {property_variable} {\n\
$self createColorPalette .colorpalette \\\n\
{00 33 66 99 CC FF} \\\n\
{00 33 66 99 CC FF} \\\n\
{00 33 66 99 CC FF} \\\n\
.colorsp\n\
}\n\
\n\
Editor instproc addTimeListItem {time_menu textbox} {\n\
$self instvar start_stop_time_list_\n\
$self tkvar start_stop_time_ \n\
\n\
set value [$textbox get 0.0 1.end]\n\
\n\
\n\
set duplicate \"no\"\n\
foreach time $start_stop_time_list_ {\n\
if {$value == $time} {\n\
set duplicate \"yes\"\n\
}\n\
}\n\
\n\
if {$duplicate != \"yes\"} {\n\
set add_list [lsort -real [linsert $start_stop_time_list_ 0 $value]]\n\
\n\
$self generateTimeListMenu $time_menu $add_list\n\
\n\
set start_stop \"start\"\n\
foreach time $start_stop_time_list_ {\n\
if {$value == $time} {\n\
set start_stop_time_ \"$start_stop $time\"\n\
}\n\
\n\
if {$start_stop == \"start\"} {\n\
set start_stop \"stop\"\n\
} else {\n\
set start_stop \"start\"\n\
}\n\
}\n\
}\n\
}\n\
Editor instproc deleteTimeListItem {time_menu} {\n\
$self instvar start_stop_time_list_\n\
$self tkvar start_stop_time_ \n\
\n\
set time [lindex $start_stop_time_ 1]\n\
set index [lsearch $start_stop_time_list_ $time]\n\
\n\
$self generateTimeListMenu $time_menu \\\n\
[lreplace $start_stop_time_list_ $index $index]\n\
\n\
set start_stop_time_ \"start [lindex $start_stop_time_list_ 0]\"\n\
}\n\
\n\
\n\
Editor instproc generateTimeListMenu {list_menu list_values} {\n\
$self instvar start_stop_time_list_\n\
\n\
$list_menu delete 0 end\n\
\n\
set start_stop \"start\"\n\
foreach time_value $list_values {\n\
$list_menu add radiobutton -label \"$start_stop $time_value\" \\\n\
-variable start_stop_time_ \\\n\
-command \"set [$self tkvarname start_stop_time_] \\\n\
\\\"$start_stop $time_value\\\"\"\n\
if {$start_stop == \"start\"} {\n\
set start_stop \"stop\"\n\
} else {\n\
set start_stop \"start\"\n\
}\n\
}\n\
\n\
set start_stop_time_list_ $list_values\n\
}\n\
\n\
Editor instproc displaySimulatorProperties {window_name} {\n\
$self instvar SharedEnv netmodel_\n\
$netmodel_ instvar maximum_simulation_time_\n\
\n\
set window $window_name\n\
if {[winfo exists $window]} {\n\
wm deiconify $window\n\
raise $window\n\
return\n\
}\n\
\n\
eval toplevel $window $SharedEnv(PrivateCmap)\n\
\n\
wm protocol $window WM_DELETE_WINDOW \"wm withdraw $window\"\n\
\n\
frame $window.properties\n\
\n\
label $window.properties.title \\\n\
-wraplength 200 \\\n\
-text \"Simulator Properties\"\n\
pack $window.properties.title -side top\n\
\n\
frame $window.properties.simulation_time\n\
label $window.properties.simulation_time.label \\\n\
-wraplength 300 -text \"Maximum Simulation Time (seconds)\"\n\
text $window.properties.simulation_time.value \\\n\
-width 10 -height 1\n\
$window.properties.simulation_time.value insert \\\n\
0.0 $maximum_simulation_time_\n\
pack $window.properties.simulation_time.label -side left\n\
pack $window.properties.simulation_time.value -side right\n\
\n\
pack $window.properties.simulation_time -side top\n\
\n\
frame $window.buttons\n\
\n\
button $window.buttons.apply -text \"Apply\"\\\n\
-command \"$self setSimulatorProperties $window; \\\n\
destroy $window\"\n\
button $window.buttons.cancel -text \"Cancel\" \\\n\
-command \"destroy $window\"\n\
\n\
pack $window.buttons.apply $window.buttons.cancel \\\n\
-side left -padx 1m -pady 1m\n\
\n\
pack $window.buttons -side bottom -padx 1m -pady 1m\n\
pack $window.properties -side top\n\
}\n\
\n\
Editor instproc setSimulatorProperties {window} {\n\
$self instvar netmodel_ editorwindow_ slider_model_ slider_view_\n\
$netmodel_ instvar maximum_simulation_time_\n\
\n\
set maximum_simulation_time_ \\\n\
[$window.properties.simulation_time.value get 0.0 1.end]\n\
$slider_model_ destroy\n\
$slider_view_ destroy\n\
\n\
$self buildTimeSlider $editorwindow_.timeslider\n\
}\n\
\n\
Editor instproc setDropDownValue {value} {\n\
$self tkvar drop_down_value_\n\
\n\
set drop_down_value_ $value\n\
}\n\
\n\
\n\
Editor instproc reloadObjectProperties {display_frame x y} {\n\
$self instvar editor_view_ editorwindow_\n\
$self tkvar drop_down_value_\n\
\n\
$self undisplayObjectProperties $display_frame\n\
\n\
set objinfo [$editor_view_ getObjectProperty $x $y $drop_down_value_]\n\
if {[string compare $objinfo \"NONE\"] == 0 } {\n\
return \n\
}\n\
\n\
set item [lindex $objinfo 0]\n\
set item [lindex $item 0]\n\
$self displayObjectProperties $editorwindow_.properties $item $x $y $objinfo\n\
place_frame $editorwindow_ $editorwindow_.properties $x $y\n\
}\n\
\n\
Editor instproc displayObjectProperties {display_frame item x y properties} {\n\
$self instvar SharedEnv colorarea property_values textwidgets drop_down_list_\n\
$self tkvar current_time_ start_stop_time_ drop_down_value_\n\
\n\
set SharedEnv($item) $properties\n\
\n\
$self undisplayObjectProperties $display_frame\n\
frame $display_frame -relief raised -borderwidth 1 \n\
\n\
set number 0\n\
\n\
foreach property $properties {\n\
set property_label [lindex $property 0]\n\
set property_variable [lindex $property 1]\n\
set property_type [lindex $property 2]\n\
set property_value [lindex $property 3]\n\
\n\
set property_values($property_variable) $property_value \n\
\n\
frame $display_frame.line_$number\n\
\n\
if {$property_type == \"title\"} {\n\
label $display_frame.line_$number.title -wraplength 200 \\\n\
-text $property_label\n\
pack $display_frame.line_$number.title -side top\n\
\n\
\n\
} elseif {$property_type == \"text\"} { \n\
label $display_frame.line_$number.label_${property_variable} \\\n\
-wraplength 300 -text \"$property_label\"\n\
text $display_frame.line_$number.input_${property_variable} \\\n\
-width 10 -height 1\n\
set textwidgets($property_variable) \\\n\
$display_frame.line_$number.input_${property_variable}\n\
\n\
$display_frame.line_$number.input_${property_variable} insert \\\n\
0.0 $property_value\n\
\n\
pack $display_frame.line_$number.label_${property_variable} -side left\n\
pack $display_frame.line_$number.input_${property_variable} -side right\n\
\n\
\n\
} elseif {$property_type == \"color\"} {\n\
button $display_frame.line_$number.label_${property_variable} \\\n\
-text \"$property_label\" \\\n\
-command \"$self showPalette $display_frame.line_$number.input_${property_variable}\"\n\
text $display_frame.line_$number.input_${property_variable} \\\n\
-width 10 -height 1\n\
set textwidgets($property_variable) \\\n\
$display_frame.line_$number.input_${property_variable}\n\
\n\
$display_frame.line_$number.input_${property_variable} insert \\\n\
0.0 $property_value\n\
\n\
pack $display_frame.line_$number.label_${property_variable} -side left\n\
pack $display_frame.line_$number.input_${property_variable} -side right\n\
\n\
set colorarea $textwidgets($property_variable)\n\
pack $display_frame.line_$number.input_${property_variable} -side right\n\
\n\
\n\
} elseif {$property_type == \"label\"} {\n\
label $display_frame.line_$number.label_${property_variable} \\\n\
-wraplength 200 -text \"$property_label\"\n\
pack $display_frame.line_$number.label_${property_variable} -side left\n\
\n\
\n\
} elseif {$property_type == \"timelist\"} {\n\
label $display_frame.line_$number.label_${property_variable} \\\n\
-wraplength 100 -text \"$property_label\"\n\
\n\
button $display_frame.line_$number.set_${property_variable} \\\n\
-text \"Grab\" \\\n\
-command \"$self insertTimeInto \\\n\
$display_frame.line_$number.input_${property_variable}\"\n\
\n\
text $display_frame.line_$number.input_${property_variable} \\\n\
-width 10 -height 1\n\
$display_frame.line_$number.input_${property_variable} insert \\\n\
0.0 [lindex $property_value 0]\n\
\n\
set time_menu $display_frame.line_$number.timelist_${property_variable}.menu\n\
\n\
button $display_frame.line_$number.add_${property_variable} \\\n\
-text \"Add\" -command \"$self addTimeListItem $time_menu \\\n\
$display_frame.line_$number.input_${property_variable}\"\n\
\n\
set start_stop_time_ \" \"\n\
menubutton $display_frame.line_$number.timelist_${property_variable} \\\n\
-textvariable [$self tkvarname start_stop_time_] \\\n\
-text $start_stop_time_ \\\n\
-indicatoron 1 -menu $time_menu \\\n\
-relief raised -bd 2 -highlightthickness 2 -anchor c \\\n\
-direction flush -width 15\n\
\n\
menu $time_menu -tearoff 0\n\
$self generateTimeListMenu $time_menu $property_value\n\
set start_stop_time_ \"start [lindex $property_value 0]\"\n\
\n\
\n\
button $display_frame.line_$number.remove_${property_variable} \\\n\
-text \"Remove\" -command \"$self deleteTimeListItem $time_menu\"\n\
\n\
pack $display_frame.line_$number.label_${property_variable} -side left\n\
pack $display_frame.line_$number.set_${property_variable} \\\n\
$display_frame.line_$number.input_${property_variable} -side left\n\
pack $display_frame.line_$number.remove_${property_variable} \\\n\
$display_frame.line_$number.timelist_${property_variable} \\\n\
$display_frame.line_$number.add_${property_variable} -side right\n\
\n\
} elseif {$property_type == \"time\"} {\n\
text $display_frame.line_$number.input_${property_variable} \\\n\
-width 10 -height 1\n\
set textwidgets($property_variable) \\\n\
$display_frame.line_$number.input_${property_variable}\n\
\n\
button $display_frame.line_$number.label_${property_variable} \\\n\
-text \"$property_label\" \\\n\
-command \"$self insertTimeInto \\\n\
$display_frame.line_$number.input_${property_variable}\"\n\
\n\
\n\
$display_frame.line_$number.input_${property_variable} insert \\\n\
0.0 $property_value\n\
\n\
pack $display_frame.line_$number.label_${property_variable} -side left\n\
pack $display_frame.line_$number.input_${property_variable} -side right\n\
\n\
\n\
} elseif {$property_type == \"drop_down_list\"} {\n\
set drop_down_value_ [lindex $property_value 0]\n\
set drop_down_list_ [lindex $property_value 1]\n\
\n\
set line_frame $display_frame.line_$number\n\
\n\
label $line_frame.label_${property_variable} \\\n\
-wraplength 100 -text \"$property_label\"\n\
\n\
menubutton $line_frame.list_${property_variable} \\\n\
-textvariable [$self tkvarname drop_down_value_] \\\n\
-text $drop_down_value_ \\\n\
-indicatoron 1 -menu $line_frame.list_${property_variable}.menu \\\n\
-relief raised -bd 2 -highlightthickness 2 -anchor c \\\n\
-direction flush -width 17\n\
\n\
menu $line_frame.list_${property_variable}.menu -tearoff 0\n\
foreach i $drop_down_list_ {\n\
if {$i == \"-\"} {\n\
$line_frame.list_${property_variable}.menu add separator\n\
} else {\n\
$line_frame.list_${property_variable}.menu add radiobutton -label $i \\\n\
-variable drop_down_value_ \\\n\
-command \"$self setDropDownValue $i; $self reloadObjectProperties $display_frame $x $y\" \n\
}\n\
}\n\
$self setDropDownValue [lindex $property_value 0]\n\
\n\
pack $line_frame.label_${property_variable} -side left\n\
pack $line_frame.list_${property_variable} -side right\n\
\n\
} elseif {$property_type == \"checkbox\"} {\n\
checkbutton $display_frame.line_$number.checkbox_${property_variable} \\\n\
-text \"$property_label\" -variable $property_variable\n\
\n\
\n\
pack $display_frame.line_$number.checkbox_${property_variable} -side left\n\
}\n\
\n\
\n\
pack $display_frame.line_$number -side top -fill x \n\
incr number\n\
} ;# End foreach\n\
\n\
\n\
frame $display_frame.buttons\n\
button $display_frame.buttons.apply -text \"Apply\"\\\n\
-command \"$self setObjectProperties {$properties}; \\\n\
$self undisplayObjectProperties $display_frame\"\n\
button $display_frame.buttons.cancel -text \"Cancel\" \\\n\
-command \"$self undisplayObjectProperties $display_frame\"\n\
\n\
pack $display_frame.buttons.apply $display_frame.buttons.cancel -side left \\\n\
-padx 1m -pady 1m\n\
pack $display_frame.buttons -side bottom -padx 1m -pady 1m\n\
}\n\
\n\
\n\
Editor instproc insertTimeInto {textbox} {\n\
$self tkvar current_time_    \n\
$textbox delete 0.0 end\n\
$textbox insert 0.0 $current_time_\n\
}\n\
\n\
Editor instproc undisplayObjectProperties {display_frame} {\n\
if {[winfo exists $display_frame]} {\n\
foreach widget [pack slaves $display_frame] {\n\
set subwidgets [pack slaves $widget]\n\
if {[llength $subwidgets] > 0} {\n\
pack forget $subwidgets\n\
destroy $subwidgets\n\
}\n\
pack forget $widget\n\
destroy $widget\n\
}\n\
}\n\
catch {destroy $display_frame}\n\
}\n\
\n\
Editor instproc setObjectProperties {properties} {\n\
$self instvar editor_view_ property_values textwidgets \\\n\
start_stop_time_list_\n\
$self tkvar drop_down_value_\n\
\n\
foreach property $properties {\n\
\n\
set property_label [lindex $property 0]\n\
set property_variable [lindex $property 1]\n\
set property_type [lindex $property 2]\n\
\n\
if {$property_type == \"title\"} {\n\
set property_value [lindex $property 3]\n\
set type [lindex $property_value 0]\n\
set typeid [lindex $property_value 1] \n\
\n\
} elseif {$property_type == \"label\"} {\n\
\n\
} elseif {$property_type == \"timelist\"} {\n\
set property_value $start_stop_time_list_\n\
\n\
} elseif {$property_type == \"drop_down_list\"} {\n\
set property_value $drop_down_value_\n\
\n\
} elseif {$property_type == \"checkbox\"} {\n\
\n\
} else {\n\
set property_value [$textwidgets($property_variable) get 0.0 1.end]\n\
}\n\
\n\
switch \"$type\" {\n\
Node {\n\
$editor_view_ setNodeProperty $typeid \\\n\
$property_value \\\n\
$property_variable\n\
}\n\
\n\
Agent {\n\
$editor_view_ setAgentProperty $typeid \\\n\
$property_value \\\n\
$property_variable\n\
}\n\
\n\
Link {\n\
set src_dst [split $typeid \"-\"]\n\
set src [lindex $src_dst 0]\n\
set dst [lindex $src_dst 1]\n\
$editor_view_ setLinkProperty $src $dst \\\n\
$property_value \\\n\
$property_variable\n\
}\n\
\n\
QueueHandle {\n\
set src_dst [split $typeid \"-\"]\n\
set source [lindex $src_dst 0]\n\
set destination [lindex $src_dst 1]\n\
$editor_view_ setQueueHandleProperty $source $destination \\\n\
$property_value \\\n\
$property_variable\n\
}\n\
\n\
TrafficSource {\n\
$editor_view_ setTrafficSourceProperty $typeid \\\n\
$property_value \\\n\
$property_variable\n\
}\n\
\n\
\n\
LossModel {\n\
$editor_view_ setLossModelProperty $typeid \\\n\
$property_value \\\n\
$property_variable\n\
}\n\
\n\
}\n\
}\n\
}\n\
\n\
Editor instproc buildToolbar {view toolbar} {\n\
$self instvar magnification viewOffset SharedEnv balloon_ \\\n\
state_buttons_ edit_state_ editorwindow_ agent_types_ \\\n\
source_types_ toolbar_buttons_ current_tool_ \\\n\
lossmodel_types_\n\
$self tkvar current_agent_ current_source_ current_lossmodel_\n\
\n\
frame $toolbar.leftside\n\
\n\
button $toolbar.leftside.select -bitmap \"select\" \\\n\
-command \"$self chooseTool select\" \\\n\
-highlightthickness 0 -borderwidth 1\n\
set toolbar_buttons_(select) $toolbar.leftside.select\n\
\n\
button $toolbar.leftside.addnode -bitmap \"addnode\" \\\n\
-command \" $self chooseTool addnode\" \\\n\
-highlightthickness 0 -borderwidth 1\n\
set toolbar_buttons_(addnode) $toolbar.leftside.addnode\n\
\n\
button $toolbar.leftside.addlink -bitmap \"addlink\" \\\n\
-command \"$self chooseTool addlink\" \\\n\
-highlightthickness 0 -borderwidth 1\n\
set toolbar_buttons_(addlink) $toolbar.leftside.addlink\n\
\n\
button $toolbar.leftside.addagent -text \"Agent\" \\\n\
-command \"$self chooseTool addagent\" \\\n\
-highlightthickness 0 -borderwidth 1\n\
set toolbar_buttons_(addagent) $toolbar.leftside.addagent\n\
\n\
menubutton $toolbar.leftside.agent_list -textvariable [$self tkvarname current_agent_] \\\n\
-text $current_agent_ \\\n\
-indicatoron 1 -menu $toolbar.leftside.agent_list.menu \\\n\
-relief raised -bd 2 -highlightthickness 2 -anchor c \\\n\
-direction flush\n\
menu $toolbar.leftside.agent_list.menu -tearoff 0\n\
foreach i $agent_types_ {\n\
if {$i == \"-\"} {\n\
$toolbar.leftside.agent_list.menu add separator\n\
} else {\n\
$toolbar.leftside.agent_list.menu add radiobutton -label $i \\\n\
-variable current_agent_ \\\n\
-command \"$self chooseAgent $i\" \n\
}\n\
}\n\
\n\
$toolbar.leftside.agent_list configure -width 16\n\
\n\
button $toolbar.leftside.add_traffic_source -text \"Traffic Source\" \\\n\
-command \"$self chooseTool addtrafficsource\" \\\n\
-highlightthickness 0 -borderwidth 1\n\
set toolbar_buttons_(addtrafficsource) $toolbar.leftside.add_traffic_source\n\
\n\
menubutton $toolbar.leftside.source_list -textvariable [$self tkvarname current_source_] \\\n\
-indicatoron 1 -menu $toolbar.leftside.source_list.menu \\\n\
-relief raised -bd 2 -highlightthickness 2 -anchor c \\\n\
-direction flush\n\
menu $toolbar.leftside.source_list.menu -tearoff 0\n\
foreach i $source_types_ {\n\
$toolbar.leftside.source_list.menu add radiobutton -label $i \\\n\
-variable current_source_ \\\n\
-command \"$self chooseTrafficSource $i\" \n\
}\n\
$toolbar.leftside.source_list configure -width 16\n\
\n\
button $toolbar.leftside.add_lossmodel -text \"Loss Model\" \\\n\
-command \"$self chooseTool addlossmodel\" \\\n\
-highlightthickness 0 -borderwidth 1\n\
set toolbar_buttons_(addlossmodel) $toolbar.leftside.add_lossmodel\n\
\n\
menubutton $toolbar.leftside.lossmodel_list \\\n\
-textvariable [$self tkvarname current_lossmodel_] \\\n\
-indicatoron 1 -menu $toolbar.leftside.lossmodel_list.menu \\\n\
-relief raised -bd 2 -highlightthickness 2 -anchor c \\\n\
-direction flush\n\
menu $toolbar.leftside.lossmodel_list.menu -tearoff 0\n\
foreach i $lossmodel_types_ {\n\
$toolbar.leftside.lossmodel_list.menu add radiobutton -label $i \\\n\
-variable current_lossmodel_ \\\n\
-command \"$self chooseLossModel $i\" \n\
}\n\
$toolbar.leftside.lossmodel_list configure -width 16\n\
\n\
button $toolbar.leftside.deleteobject -bitmap \"delete\" \\\n\
-command \"$self chooseTool deleteobject\" \\\n\
-highlightthickness 0 -borderwidth 1\n\
set toolbar_buttons_(deleteobject) $toolbar.leftside.deleteobject\n\
\n\
pack $toolbar.leftside.select \\\n\
$toolbar.leftside.addnode \\\n\
$toolbar.leftside.addlink \\\n\
$toolbar.leftside.addagent \\\n\
$toolbar.leftside.agent_list \\\n\
$toolbar.leftside.add_traffic_source \\\n\
$toolbar.leftside.source_list \\\n\
$toolbar.leftside.add_lossmodel \\\n\
$toolbar.leftside.lossmodel_list \\\n\
$toolbar.leftside.deleteobject \\\n\
-side left\n\
\n\
pack $toolbar.leftside -side left \n\
\n\
$balloon_ balloon_for $toolbar.leftside.select \"Select Object, Move Object or Get Object Properties\"\n\
$balloon_ balloon_for $toolbar.leftside.addnode \"Add Node\"\n\
$balloon_ balloon_for $toolbar.leftside.addlink \"Add Link or Connect Agents\"\n\
$balloon_ balloon_for $toolbar.leftside.addagent \"Add Agent to a Node\"\n\
$balloon_ balloon_for $toolbar.leftside.agent_list \"Choose Which Agent to Add\"\n\
$balloon_ balloon_for $toolbar.leftside.add_traffic_source \"Add Traffic Source\"\n\
$balloon_ balloon_for $toolbar.leftside.source_list \"Choose Which Traffic Source to Add\"\n\
$balloon_ balloon_for $toolbar.leftside.add_lossmodel \"Add Link Loss Model\"\n\
$balloon_ balloon_for $toolbar.leftside.lossmodel_list \"Choose Which Loss Model to Add\"\n\
$balloon_ balloon_for $toolbar.leftside.deleteobject \"Delete Object\"\n\
}\n\
\n\
\n\
Editor instproc buildTimeControls {w} {\n\
$self instvar rateSlider granularity time_step_ stepDisp running \\\n\
direction balloon_ current_time_step_ previous_time_step_\n\
$self tkvar current_time_\n\
\n\
set current_time_step_ 0.0001\n\
\n\
set f [smallfont]\n\
frame $w.bar -relief groove -borderwidth 2\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
label $w.bar.timer_label -text \"Time :\" \\\n\
-anchor w -font $f -borderwidth 1 \\\n\
-relief sunken -anchor e\n\
label $w.bar.timer_value -textvariable [$self tkvarname current_time_] \\\n\
-width 14 -anchor w -font $f -borderwidth 1 \\\n\
-relief sunken -anchor e\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
pack $w.bar.timer_value $w.bar.timer_label -side right -pady 0 \\\n\
-ipady 1 -padx 1 -fill y\n\
\n\
pack $w.bar -fill x -expand 1 -side right\n\
\n\
\n\
}\n\
\n\
Editor instproc buildTimeSlider {w} {\n\
$self tkvar showpanel\n\
$self instvar mintime_ now_ slider_model_ pipemode_ slider_view_ netmodel_\n\
$netmodel_ instvar maximum_simulation_time_ ;# This is set by editornetmodel.cc\n\
\n\
set mintime_ 0.0\n\
set now_ 0.0\n\
\n\
set slider_model_ [new TimesliderModel $mintime_ $maximum_simulation_time_ $now_ $self]\n\
set slider_view_ [new TimesliderView $w $slider_model_]\n\
\n\
$slider_model_ addObserver $slider_view_\n\
$slider_model_ setpipemode 0\n\
\n\
$self setCurrentTime 0.0\n\
}\n\
\n\
Editor instproc setsliderPressed {v} {\n\
}\n\
\n\
Editor instproc setCurrentTime {time} {\n\
$self tkvar current_time_\n\
\n\
set current_time_ $time\n\
$self renderFrame\n\
}\n\
\n\
Editor instproc slidetime {a b} {\n\
}\n\
\n\
Editor instproc fast_fwd {} {\n\
\n\
}\n\
\n\
Editor instproc rewind {} {\n\
}\n\
\n\
Editor instproc setsliderPressed {b} {\n\
}\n\
\n\
Editor instproc getrunning {} {\n\
return 1\n\
}\n\
\n\
\n\
Editor instproc renderFrame {} {\n\
$self instvar editor_view_ netmodel_ slider_view_\n\
$self tkvar current_time_\n\
\n\
$netmodel_ updateNodePositions $current_time_\n\
$editor_view_ draw $current_time_\n\
}\n\
\n\
\n\
Editor instproc addTimeSliderNodeMarkers {marker_list} {\n\
$self instvar slider_view_\n\
foreach line $marker_list {\n\
set node_id [lindex $line 0]\n\
foreach time [lrange $line 1 end] {\n\
$slider_view_ addMarker $time $node_id seagreen\n\
}\n\
}\n\
}\n\
\n\
\n\
\n\
Editor instproc buildMenubar {w} {\n\
$self instvar editorwindow_ menubar_ agent_types_ \\\n\
source_types_ lossmodel_types_\n\
\n\
$self tkvar current_agent_ current_source_ current_lossmodel_\n\
menubutton $w.file -text File -underline 0 \\\n\
-menu $w.file.menu\n\
menu $w.file.menu\n\
$w.file.menu add command -label New \\\n\
-underline 0 -command \"$self newFile\"\n\
$w.file.menu add command -label Open \\\n\
-underline 0 -command \"$self openFile\"\n\
$w.file.menu add command -label Save \\\n\
-underline 0 -command \"$self doSave\"\n\
$w.file.menu add command -label \"Save As...\" \\\n\
-underline 4 -command \"$self doSaveAs\"\n\
$w.file.menu add separator\n\
$w.file.menu add command -label \"Run Ns\" \\\n\
-underline 1 -command \"$self doSave; $self runNs\"\n\
$w.file.menu add command -label \"Close\" \\\n\
-underline 0 -command \"destroy $editorwindow_\"\n\
\n\
menubutton $w.edit -text Edit -underline 0 \\\n\
-menu $w.edit.menu\n\
menu $w.edit.menu\n\
$w.edit.menu add command -label \"Undo\" \\\n\
-underline 0 -command \"puts Undo\" -state disabled\n\
$w.edit.menu add command -label \"Redo\" \\\n\
-underline 0 -command \"puts Redo\" -state disabled\n\
$w.edit.menu add separator\n\
$w.edit.menu add command -label \"Cut\" \\\n\
-underline 0 -command \"puts Cut\" -state disabled\n\
$w.edit.menu add command -label \"Copy\" \\\n\
-underline 0 -command \"puts Copy\" -state disabled\n\
$w.edit.menu add command -label \"Paste\" \\\n\
-underline 0 -command \"puts Paste\" -state disabled\n\
$w.edit.menu add separator\n\
$w.edit.menu add command -label \"Simulator Properties\" \\\n\
-underline 0 -command \"$self displaySimulatorProperties .properties\"\n\
\n\
menubutton $w.tools -text Tools -underline 0 \\\n\
-menu $w.tools.menu\n\
menu $w.tools.menu\n\
$w.tools.menu add command -label \"Select\" \\\n\
-underline 0 -command \"$self chooseTool select\"\n\
$w.tools.menu add command -label \"Add Node\" \\\n\
-underline 0  \\\n\
-command \"$self chooseTool addnode\"\n\
$w.tools.menu add command -label \"Add Link\" \\\n\
-underline 0  \\\n\
-command \"$self chooseTool addlink\"\n\
\n\
$w.tools.menu add cascade -label \"Agents\" \\\n\
-underline 0 -menu $w.tools.menu.agents\n\
menu $w.tools.menu.agents\n\
foreach i $agent_types_ {\n\
if {$i == \"-\"} {\n\
$w.tools.menu.agents add separator\n\
} else {\n\
$w.tools.menu.agents add radiobutton \\\n\
-label $i -variable current_agent_ -value $i \\\n\
-command \"$self chooseAgent $i\"\n\
}\n\
}\n\
\n\
$w.tools.menu add cascade -label \"Traffic Sources\" \\\n\
-underline 0 -menu $w.tools.menu.sources\n\
menu $w.tools.menu.sources\n\
foreach i $source_types_ {\n\
$w.tools.menu.sources add radiobutton \\\n\
-label $i -variable current_source_ -value $i \\\n\
-command \"$self chooseTrafficSource $i\"\n\
}\n\
\n\
$w.tools.menu add cascade -label \"Link Loss Models\" \\\n\
-underline 0 -menu $w.tools.menu.lossmodels\n\
menu $w.tools.menu.lossmodels\n\
foreach i $lossmodel_types_ {\n\
$w.tools.menu.lossmodels add radiobutton \\\n\
-label $i -variable current_lossmodel_ -value $i \\\n\
-command \"$self chooseLossModel $i\"\n\
}\n\
\n\
$w.tools.menu add command -label \"Delete Object\" \\\n\
-underline 0 -command \"$self chooseTool deleteobject\"\n\
\n\
\n\
pack $w.file $w.edit \\\n\
$w.tools -side left   \n\
\n\
menubutton $w.help -text \"Help\" -underline 0 \\\n\
-menu $w.help.menu\n\
menu $w.help.menu\n\
$w.help.menu add command -label \"Help\" \\\n\
-underline 0 -command \"$self showHelp\"\n\
pack $w.help -side right\n\
\n\
\n\
}\n\
\n\
Editor instproc setMessage {} {\n\
$self instvar SharedEnv\n\
return $SharedEnv(Cursor)\n\
}\n\
\n\
Editor instproc originObject { name } {\n\
$self instvar SharedEnv\n\
\n\
set bb [$SharedEnv(CurrentCanvas) bbox $name]\n\
set x [lindex $bb 0]\n\
set y [lindex $bb 1]\n\
if {$x > 0} {\n\
set nx -$x\n\
} else {\n\
set nx [expr abs($x)]\n\
}\n\
if {$y > 0} {\n\
set ny -$y              \n\
} else {         \n\
set ny [expr abs($y)]\n\
}       \n\
$SharedEnv(CurrentCanvas) move $name $nx $ny\n\
}    \n\
\n\
Editor instproc scaleObject {name ratio width height} {\n\
$self instvar SharedEnv\n\
\n\
set bb [eval $SharedEnv(CurrentCanvas) bbox $name]\n\
if {\"$bb\" != \"\"} {\n\
set x [lindex $bb 2]\n\
set y [lindex $bb 3]\n\
if {$ratio == \"variable\"} {\n\
set scalex [expr ($width + 0.0) / $x]\n\
set scaley [expr ($height + 0.0) / $y]\n\
if {$scalex > $scaley} {\n\
set scale $scaley\n\
} else {\n\
set scale $scalex\n\
}\n\
} elseif {$ratio != \"\"} {\n\
set scalex $ratio\n\
set scaley $ratio\n\
set scale $ratio\n\
} else {\n\
set scalex [expr ($width + 0.0) / $x]\n\
set scaley [expr ($height + 0.0) / $y]\n\
if {$scalex > $scaley} {\n\
set scalex $scaley\n\
set scale $scaley\n\
} else {\n\
set scaley $scalex\n\
set scale $scalex\n\
}\n\
}\n\
$SharedEnv(CurrentCanvas) scale $name 0 0 $scalex $scaley\n\
foreach i [$SharedEnv(CurrentCanvas) find withtag all] {\n\
set type [$SharedEnv(CurrentCanvas) type $i]\n\
if {\"$type\" != \"text\"} {\n\
continue\n\
}\n\
if {$SharedEnv(FontScale)} {\n\
set fn [$SharedEnv(CurrentCanvas) itemcget $i -font]\n\
regexp \\\n\
{([-][^-]*-[^-]*-[^-]*-[^-]*-[^-]*-[^-]*-[^-]*-)([^-]*)(-.*)}\\\n\
$fn dummy d1 size d2;\n\
if {\"$dummy\" != \"\"} {\n\
set nsize [expr round($size * $scale)]\n\
if {$nsize < 20} {\n\
set nsize 20\n\
}\n\
$SharedEnv(CurrentCanvas) itemconfigure $i \\\n\
-font ${d1}${nsize}${d2}\n\
set fnn [$SharedEnv(CurrentCanvas) itemcget $i -font]\n\
regexp \\\n\
{([-][^-]*-[^-]*-[^-]*-[^-]*-[^-]*-[^-]*-[^-]*-)([^-]*)(-.*)}\\\n\
$fnn dummy d1 nsize d2;\n\
if { ($scale < 1 && $nsize < $size ) || \\\n\
($scale > 1 && $nsize > $size) } {\n\
$SharedEnv(CurrentCanvas) itemconfigure $i \\\n\
-width [expr [$SharedEnv(CurrentCanvas) itemcget $i \\\n\
-width] * $scale]\n\
} else {\n\
$SharedEnv(CurrentCanvas) itemconfigure $i \\\n\
-font $fn\n\
}\n\
}\n\
} else {\n\
$SharedEnv(CurrentCanvas) itemconfigure $i \\\n\
-width [expr [$SharedEnv(CurrentCanvas) itemcget $i -width] \\\n\
* $scale]\n\
}\n\
}\n\
}\n\
}\n\
\n\
\n\
\n\
Editor instproc setCursor { value } {                              \n\
$self instvar SharedEnv                                   \n\
\n\
set SharedEnv(Cursor) $value                       \n\
$self setRadioButton SharedEnv(Cursor) $value    \n\
}\n\
\n\
Editor instproc setRadioButton { var value } {\n\
$self instvar SharedEnv\n\
\n\
if {[info exists SharedEnv(W_${value})]} {\n\
set bd [$SharedEnv(W_${value}) cget -bd]\n\
$self clearRadioVariable $var\n\
$SharedEnv(W_${value}) configure -relief sunken\n\
$SharedEnv(W_${value}) move all $bd $bd\n\
set SharedEnv($var) $SharedEnv(W_${value})\n\
}        \n\
set $var \"$value\"       \n\
}\n\
\n\
Editor instproc clearRadioVariable {var} { \n\
$self instvar SharedEnv           \n\
if {[info exists SharedEnv($var)] && [winfo exists $SharedEnv($var)]} {\n\
set bd [$SharedEnv($var) cget -bd]\n\
$SharedEnv($var) configure -relief $SharedEnv(ButtonRelief)\n\
eval $SharedEnv($var) move all -$bd -$bd\n\
}       \n\
set $var \"\"  \n\
}\n\
\n\
Editor instproc enterObject { w x y value } {\n\
catch {destroy $w.f}\n\
frame $w.f -relief groove -borderwidth 2\n\
message $w.f.msg -width 8c -text $value\n\
pack $w.f.msg\n\
pack $w.f\n\
catch {place_frame $w $w.f [expr $x+10] [expr $y+10]}\n\
}\n\
\n\
\n\
Editor instproc enterToolButtons { w x y value } {\n\
$self instvar SharedEnv\n\
set geo [winfo geometry $w]\n\
set px [lindex [split $geo \"+\"] 1]\n\
set py [lindex [split $geo \"+\"] 2]\n\
set psize [lindex [split $geo \"+\"] 0]\n\
set pxsize [lindex [split $psize \"x\"] 0]\n\
set pysize [lindex [split $psize \"x\"] 1]\n\
\n\
set w [winfo parent $w]\n\
\n\
catch {destroy $w.fm}\n\
\n\
frame $w.fm -relief groove -borderwidth 2\n\
message $w.fm.msg -width 10c -text $value -background yellow\n\
pack $w.fm.msg\n\
pack $w.fm \n\
\n\
catch {place_frame $w $w.fm [expr $pxsize+$px] [expr $pysize+$py]}\n\
}\n\
\n\
Editor instproc leaveToolButtons { w } {\n\
set w [winfo parent $w]\n\
catch {destroy $w.fm} \n\
}\n\
\n\
\n\
Editor instproc createColorPalette {name redlist greenlist bluelist replace} {\n\
$self instvar SharedEnv netmodel_\n\
\n\
set w ${name}\n\
if {[winfo exists $w]} {\n\
wm deiconify $w\n\
raise $w\n\
return\n\
}\n\
\n\
set SharedEnv($name) $replace\n\
eval toplevel $w $SharedEnv(PrivateCmap)\n\
\n\
wm protocol $w WM_DELETE_WINDOW \"wm withdraw $w\"\n\
\n\
frame $w.f\n\
\n\
foreach red $redlist {\n\
frame $w.f.rcol_${red}\n\
foreach green $greenlist {\n\
frame $w.f.grow_${red}${green}\n\
foreach blue $bluelist {\n\
if { [info exists SharedEnv($w.f.c${red}${green}${blue})] } {\n\
frame $w.f.c${red}${green}${blue} \\\n\
-relief raised -height 2m -width 2m \\\n\
-highlightthickness 0 \\\n\
-bd 1 -bg $SharedEnv($w.f.c${red}${green}${blue})\n\
} else {\n\
frame $w.f.c${red}${green}${blue} \\\n\
-relief raised -height 2m -width 2m \\\n\
-highlightthickness 0 \\\n\
-bd 1 -bg \"#${red}${green}${blue}\"\n\
}\n\
pack $w.f.c${red}${green}${blue} -side left \\\n\
-in $w.f.grow_${red}${green} -fill both -expand true\n\
\n\
bind $w.f.c${red}${green}${blue} <1> {\n\
%W configure -relief sunken\n\
}\n\
\n\
bind $w.f.c${red}${green}${blue} <Double-Button-1> \"$self getcolorname %W; destroy $w\"\n\
\n\
bind $w.f.c${red}${green}${blue} <ButtonRelease-1> \"$self getcolorname %W; destroy $w\"\n\
\n\
}\n\
pack $w.f.grow_${red}${green} -side top -in $w.f.rcol_${red} -fill both -expand true\n\
}\n\
pack $w.f.rcol_${red} -side left -in $w.f -fill both -expand true\n\
}\n\
\n\
frame $w.f.c_none -width 4m -relief raised -bd 1 \\\n\
-highlightthickness 0\n\
pack $w.f.c_none -in $w.f -side left -fill y \n\
\n\
pack $w.f -in $w -expand true -fill both\n\
\n\
wm geometry $w $SharedEnv(ColorWidth)x$SharedEnv(ColorHeight)\n\
}\n\
\n\
Editor instproc getcolorname {w} {\n\
$self instvar netmodel_ colorarea\n\
\n\
$w configure -relief raised\n\
set colorv [$w cget -bg] \n\
set rgb [winfo rgb $w $colorv]\n\
\n\
set colorn \"[$netmodel_ lookupColorName [lindex $rgb 0] \\\n\
[lindex $rgb 1] [lindex $rgb 2]]\"\n\
\n\
if {[info exists colorarea] && [winfo exists $colorarea]} {\n\
\n\
$colorarea delete 0.0 end\n\
$colorarea insert 0.0 $colorn \n\
catch { $colorarea tag add bgcolor 0.0 end }\n\
$colorarea tag configure bgcolor -background $colorn\n\
\n\
}\n\
\n\
}\n\
\n\
Editor instproc showHelp {} {\n\
$self instvar editorwindow_\n\
set w $editorwindow_.1\n\
if {[winfo exists $w]} {\n\
wm deiconify $w\n\
raise $w\n\
return\n\
}\n\
toplevel $w\n\
wm title $w \"Quick Tips on using the Nam Editor\"\n\
listbox $w.lbox -width 48 -height 11\n\
pack $w.lbox -side left -expand yes -fill both\n\
$w.lbox insert 0 \\\n\
\"Nam Editor Help\"\\\n\
\"-----------------------------------------------\" \\\n\
\"1) Create nodes using the add node tool \" \\\n\
\"2) Connect them using the add link tool.\" \\\n\
\"3) Attach an agent to a node\"\\\n\
\"4) Connect agents by using the link tool.\"\\\n\
\"5) Set options by double clicking or right clicking on each object.\"\\\n\
\"6) Run the scenario using the File Menu command, Run Ns.\"\\\n\
\"-----------------------------------------------\" \\\n\
\"Please report all bugs to ns-users@isi.edu\"\n\
$w.lbox selection set 0\n\
}\n\
\n\
Editor instproc chooseTool {tool} {\n\
$self instvar SharedEnv state_buttons_ toolbar_buttons_ \\\n\
current_tool_ slider_view_\n\
$self tkvar current_agent_ current_source_\n\
$slider_view_ instvar delete_marker_\n\
\n\
if [info exists current_tool_] {\n\
$toolbar_buttons_($current_tool_) configure -relief raised\n\
}\n\
\n\
set current_tool_ $tool\n\
if [info exists toolbar_buttons_($tool)] {\n\
$toolbar_buttons_($tool) configure -relief sunken\n\
}\n\
\n\
if {$tool == \"deleteobject\"} {\n\
set delete_marker_ 1\n\
} else {\n\
set delete_marker_ 0\n\
}\n\
}\n\
\n\
Editor instproc chooseAgent {agent} {\n\
$self tkvar current_agent_\n\
\n\
set current_agent_ $agent\n\
$self chooseTool addagent\n\
\n\
}\n\
\n\
Editor instproc chooseTrafficSource {source} {\n\
$self tkvar current_source_\n\
\n\
set current_source_ $source\n\
$self chooseTool addtrafficsource\n\
\n\
}\n\
\n\
Editor instproc chooseLossModel {lossmodel} {\n\
$self tkvar current_lossmodel_\n\
\n\
set current_lossmodel_ $lossmodel\n\
$self chooseTool addlossmodel\n\
\n\
}\n\
\n\
Editor instproc popupCanvasSet { type } {\n\
$self instvar SharedEnv edit_state_ state_buttons_\n\
\n\
if [info exists edit_state_] {\n\
$state_buttons_($edit_state_) configure -relief raised\n\
}\n\
set edit_state_ $type\n\
if [info exists state_buttons_($type)] {\n\
$state_buttons_($type) configure -relief sunken\n\
}\n\
\n\
$self setCursor \"$type\"\n\
switch \"$SharedEnv(Cursor)\" {\n\
Cut {\n\
set SharedEnv(Message) Cut\n\
$self setCursor \"Cut\"\n\
}\n\
Paste {\n\
if {$SharedEnv(Cut) == \"\"} {\n\
setCursor \"Select\"\n\
set SharedEnv(Message) Select\n\
enableSelectionButtons\n\
}\n\
}\n\
Link -\n\
Node -\n\
Queue -\n\
Agent -\n\
Text {\n\
set SharedEnv(Sub-Cursor) \"\"\n\
$SharedEnv(CurrentCanvas) dctag\n\
set SharedEnv(Message) $SharedEnv(Cursor)\n\
}\n\
Select {\n\
set SharedEnv(Message) $SharedEnv(Cursor)\n\
}\n\
\n\
Print {\n\
\n\
return\n\
}\n\
}\n\
focus $SharedEnv(CurrentCanvas)\n\
}\n\
\n\
Editor instproc newFile {} {\n\
set editor [new Editor \"\"]\n\
}\n\
\n\
Editor instproc openFile {} {\n\
$self instvar editorwindow_\n\
\n\
set openFile_ [tk_getOpenFile \\\n\
-filetypes {{{Nam Editor} {.ns .enam}} \\\n\
{{All Files} {*}}} \\\n\
-parent $editorwindow_]\n\
\n\
if {$openFile_ == \"\"} {\n\
return \n\
}\n\
\n\
set editor [new Editor $openFile_]\n\
return\n\
}\n\
\n\
Editor instproc doSave {} {\n\
$self instvar save_filename_\n\
\n\
if {\"$save_filename_\" != \"\"} {\n\
$self writeFile $save_filename_\n\
} else {\n\
$self doSaveAs\n\
}\n\
}\n\
\n\
Editor instproc doSaveAs {} {\n\
$self instvar save_filename_ editorwindow_\n\
\n\
set filename [tk_getSaveFile -defaultextension \".ns\" \\\n\
-filetypes {{{Nam Editor} {.ns .enam}}} \\\n\
-parent $editorwindow_]\n\
if {\"$filename\" != \"\"} {\n\
$self writeFile $filename\n\
}\n\
}\n\
\n\
Editor instproc writeFile {filename} {\n\
$self instvar netmodel_ save_filename_ editorwindow_\n\
\n\
if {\"$filename\" != \"\"} {\n\
regexp {(.*)[.]ns} $filename matched filename_root\n\
catch {$netmodel_ saveasns $filename $filename_root} error\n\
\n\
if {![info exists error]} {\n\
$self msgRaise .alert \"Nam File Error\" \"ERROR: Unable to save file: $filename.\"\n\
} else {\n\
set save_filename_ $filename\n\
wm title $editorwindow_ \"Nam Editor - $save_filename_\"\n\
update\n\
}\n\
}\n\
}\n\
\n\
Editor instproc runNs {} {\n\
$self instvar save_filename_\n\
if {$save_filename_ != \"\"} {\n\
set filename $save_filename_\n\
catch {exec ns $filename &} error\n\
if {[info exists error]} {\n\
if {[regexp {([0-9]+)} $error matched process_id]} {\n\
$self msgRaise .alert \"Nam - Ns id $process_id\" \"Ns should be running as process id $process_id.\\n\\\n\
The result will be shown when it finishes.\"\n\
} else {\n\
$self msgRaise .alert \"Nam - Ns Error\" \"Ns was unable to run.          \\n\\\n\
The error message reported was:\\n\\\n\
\\[ $error \\]\"\n\
}\n\
}\n\
}\n\
}\n\
\n\
Editor instproc msgRaise {name title message} {  \n\
$self instvar SharedEnv   \n\
\n\
set w ${name}   \n\
set SharedEnv(MsgReplace) $SharedEnv(CurrentCanvas)\n\
\n\
if {![winfo exists $name]} {\n\
$self  msgRequestor $name\n\
}\n\
$w.message configure -text \"$message\"\n\
wm title $w $title\n\
wm deiconify $w\n\
focus $w\n\
}\n\
\n\
Editor instproc msgRequestor {name} {\n\
$self instvar SharedEnv\n\
\n\
set w ${name}   \n\
toplevel $w\n\
wm protocol $w WM_DELETE_WINDOW {set dummy 1}\n\
frame $w.f -borderwidth 0 -highlightthickness 0\n\
frame $w.texttop -borderwidth 0 -highlightthickness 0\n\
frame $w.textbottom -relief raised -bd 1\n\
label $w.message\n\
button $w.ok -text \"Ok\" -command \"$self msgLower $w\"\n\
pack $w.message -in $w.texttop -fill both -expand true\n\
pack $w.ok -in $w.textbottom -side bottom -padx 2m -pady 2m\n\
pack $w.texttop $w.textbottom -anchor sw -in $w -fill x -expand true\n\
pack $w.f -in $w\n\
wm withdraw $w\n\
}\n\
\n\
Editor instproc msgLower {name} {\n\
$self instvar SharedEnv\n\
\n\
set w ${name}\n\
wm withdraw $w\n\
}\n\
\n\
\n\
Editor instproc moveToMarkerTime {time} {\n\
$self instvar slider_model_\n\
\n\
\n\
$slider_model_ setcurrenttime $time\n\
$self setCurrentTime $time\n\
}\n\
\n\
Editor instproc removeMarker {time node_id} {\n\
$self instvar netmodel_\n\
\n\
$netmodel_ removeNodeMovement $node_id $time\n\
$self renderFrame\n\
}\n\
\n\
\n\
\n\
\n\
Editor instproc loadNsScript {tracefile} {\n\
set stream [new NamStream $tracefile]\n\
\n\
while {[$stream eof] == 0} {\n\
set line [$stream gets]\n\
$self parseLine $line\n\
}\n\
}\n\
\n\
\n\
Editor instproc parseLine {line} {\n\
$self instvar node_ edge_ agent_ traffic_source_ editor_view_ fid_ \\\n\
loss_model_ netmodel_\n\
\n\
if {[regexp {## node\\(([0-9]+)\\) at (-*[0-9.]+),(-*[0-9.]+)} \\\n\
$line matched node_id x y]} {\n\
\n\
} elseif {[regexp {^set node\\(([0-9]+)\\) \\[\\$ns node\\]} $line matched node_id]} {\n\
set node_($node_id) [new Node $node_id circle 10.0]\n\
$editor_view_ importNode $node_($node_id)\n\
\n\
} elseif {[regexp {^\\$node\\(([0-9]+)\\) set ([XYZ]_) ([-.0-9]+)} \\\n\
$line matched node_id property_variable value]} {\n\
\n\
$editor_view_ setNodeProperty $node_id $value $property_variable\n\
\n\
} elseif {[regexp {^\\$node\\(([0-9]+)\\) ([a-zA-Z_]+) \\\"*([a-zA-Z.0-9#]+)\\\"*} \\\n\
$line matched node_id property_variable value]} {\n\
\n\
$editor_view_ setNodeProperty $node_id $value $property_variable\n\
\n\
} elseif {[regexp {^\\$ns at [0-9.]+ \\\"\\$node\\(([0-9]+)\\) label ([a-zA-Z_]+) \\\"} \\\n\
$line matched node_id value]} {\n\
$editor_view_ setNodeProperty $node_id $value dlabel_\n\
\n\
} elseif {[regexp {^\\$ns at ([0-9.]+) \\\"\\$node\\(([0-9]+)\\) setdest ([-.0-9]+) ([-.0-9]+) ([.0-9]+)\\\"} \\\n\
$line matched time node_id x y speed]} {\n\
$node_($node_id) addMovement $time $x $y $speed\n\
\n\
} elseif {[regexp {^\\$ns simplex-link \\$node\\(([0-9]+)\\) \\$node\\(([0-9]+)\\) ([0-9.]+)Mb ([0-9.]+)ms ([A-Za-z]+)} \\\n\
$line matched source destination bandwidth delay queue_type]} {\n\
set sd \"$source - $destination\"\n\
set edge_($sd) [new Edge $bandwidth $delay 1.0 0.0 25.0]\n\
$editor_view_ attachNodes $edge_($sd) $node_($source) $node_($destination) \n\
$editor_view_ addQueueHandle $edge_($sd) $queue_type\n\
\n\
\n\
} elseif {[regexp {^\\$ns simplex-link-op \\$node\\(([0-9]+)\\) \\$node\\(([0-9]+)\\) ([a-zA-Z_]+) ([0-9.a-zA-Z#]+)} $line matched source destination property_variable value]} {\n\
$editor_view_ setLinkProperty $source $destination $value $property_variable\n\
\n\
} elseif {[regexp {^\\[\\[\\$ns link \\$node\\(([0-9]+)\\) \\$node\\(([0-9]+)\\)\\] queue\\] set ([a-zA-Z_]+) ([0-9.a-zA-Z]+)} \\\n\
$line matched source destination variable_name value]} {\n\
$editor_view_ setQueueHandleProperty $source $destination \\\n\
$variable_name $value\n\
\n\
} elseif {[regexp {^set loss_model\\(([0-9]+)\\) \\[new ErrorModel/(Uniform) ([0-9.]+) ([a-zA-Z]+)} \\\n\
$line matched loss_model_id loss_model_type rate_ loss_unit_]} { \n\
set loss_model_($loss_model_id) [new LossModel $loss_model_type \\\n\
$loss_model_id 10.0]\n\
$loss_model_($loss_model_id) setRate $rate_ \n\
$loss_model_($loss_model_id) setLossUnit $loss_unit_\n\
\n\
} elseif {[regexp {^set loss_model\\(([0-9]+)\\) \\[new ErrorModel/([a-zA-Z]+)} \\\n\
$line matched loss_model_id loss_model_type]} { \n\
set loss_model_($loss_model_id) [new LossModel $loss_model_type \\\n\
$loss_model_id 10.0]\n\
\n\
} elseif {[regexp {^\\$loss_model\\(([0-9]+)\\) set ([a-zA-Z_]+) ([0-9.a-zA-Z]+)} \\\n\
$line matched loss_model_id property_variable value]} { \n\
$editor_view_ setLossModelProperty $loss_model_id $value $property_variable\n\
\n\
} elseif {[regexp {^\\$ns lossmodel \\$loss_model\\(([0-9]+)\\) \\$node\\(([0-9]+)\\) \\$node\\(([0-9]+)\\)} \\\n\
$line matched loss_model_id source destination]} { \n\
$editor_view_ attachLossModel $loss_model_($loss_model_id) \\\n\
$node_($source) $node_($destination) \n\
\n\
} elseif {[regexp {^set agent\\(([0-9]+)\\) \\[new Agent/([a-zA-Z]+)} \\\n\
$line matched agent_id agent_type]} { \n\
set agent_($agent_id) [new Agent $agent_type $agent_id 10.0]\n\
\n\
\n\
} elseif {[regexp {^\\$ns attach-agent \\$node\\(([0-9]+)\\) \\$agent\\(([0-9]+)\\)} \\\n\
$line matched node_id agent_id]} {\n\
$editor_view_ importAgent $agent_($agent_id) $node_($node_id)\n\
\n\
\n\
} elseif {[regexp {^\\$agent\\(([0-9]+)\\) set ([a-zA-Z_]+) ([0-9.a-zA-Z]+)} \\\n\
$line matched agent_id property_variable value]} {\n\
if {$property_variable == \"fid_\"} {\n\
$editor_view_ setAgentProperty $agent_id $fid_($value) flowcolor_\n\
} else {\n\
$editor_view_ setAgentProperty $agent_id $value $property_variable\n\
}\n\
\n\
} elseif {[regexp {^\\$ns connect \\$agent\\(([0-9]+)\\) \\$agent\\(([0-9]+)\\)} \\\n\
$line matched source_id destination_id]} {\n\
$editor_view_ linkAgents $agent_($source_id) $agent_($destination_id)\n\
\n\
\n\
\n\
} elseif {[regexp {^\\$ns color ([0-9]+) \\\"([a-zA-Z0-9#]+)\\\"} \\\n\
$line matched flow_id color]} {\n\
set fid_($flow_id) $color\n\
\n\
\n\
\n\
} elseif {[regexp {^set traffic_source\\(([0-9]+)\\) \\[new Application/Traffic/([a-zA-Z]+)} \\\n\
$line matched trafficsource_id trafficsource_type]} { \n\
set traffic_source_($trafficsource_id) \\\n\
[new TrafficSource $trafficsource_type $trafficsource_id 10.0]\n\
\n\
} elseif {[regexp {^set traffic_source\\(([0-9]+)\\) \\[new Application/([a-zA-Z]+)} \\\n\
$line matched trafficsource_id trafficsource_type]} { \n\
set traffic_source_($trafficsource_id) \\\n\
[new TrafficSource $trafficsource_type $trafficsource_id 10.0]\n\
\n\
} elseif {[regexp {^\\$traffic_source\\(([0-9]+)\\) attach-agent \\$agent\\(([0-9]+)\\)} \\\n\
$line matched trafficsource_id agent_id]} {\n\
$editor_view_ attachTrafficSource $traffic_source_($trafficsource_id) \\\n\
$agent_($agent_id)\n\
\n\
} elseif {[regexp {^\\$traffic_source\\(([0-9]+)\\) set ([a-zA-Z_]+) ([0-9.a-zA-Z]+)} \\\n\
$line matched trafficsource_id property_variable value]} {\n\
\n\
$editor_view_ setTrafficSourceProperty $trafficsource_id $value $property_variable\n\
\n\
\n\
} elseif {[regexp {^\\$ns at ([0-9.]+) \\\"\\$traffic_source\\(([0-9]+)\\) ([starop]+)\\\"} \\\n\
$line matched time trafficsource_id property_variable]} {\n\
switch \"$property_variable\" {\n\
start {\n\
$editor_view_ setTrafficSourceProperty $trafficsource_id $time start_\n\
}\n\
stop {\n\
$editor_view_ setTrafficSourceProperty $trafficsource_id $time stop_\n\
}\n\
}\n\
\n\
} elseif {[regexp {^\\$ns at ([0-9.]+) \\\"finish\\\"} $line matched finish_time]} {\n\
$netmodel_ instvar maximum_simulation_time_ \n\
$self instvar slider_model_ slider_view_ editorwindow_\n\
set maximum_simulation_time_ $finish_time\n\
$slider_model_ destroy\n\
$slider_view_ destroy\n\
$self buildTimeSlider $editorwindow_.timeslider\n\
\n\
} elseif {[regexp {^#} $line]} {\n\
} elseif {[regexp {^\\n} $line]} {\n\
} else {\n\
}\n\
}\n\
\n\
NetworkModel/Editor set Wpxmin_ 500.0\n\
NetworkModel/Editor set Wpymin_ 500.0\n\
NetworkModel/Editor set Wpxmax_ 625.0\n\
NetworkModel/Editor set Wpymax_ 625.0\n\
\n\
\n\
NetworkModel/Editor instproc init {animator tracefile} {\n\
eval $self next $animator {$tracefile}\n\
NetworkModel/Editor instvar Wpxmax_ Wpymax_  Wpxmin_ Wpymin_\n\
$self set_range $Wpxmin_ $Wpymin_ $Wpxmax_ $Wpymax_\n\
}\n\
\n\
NetworkModel/Editor instproc set_range {xmin ymin xmax ymax } {\n\
$self instvar Wpxmin_ Wpymin_ Wpxmax_ Wpymax_\n\
set Wpxmin_ $xmin\n\
set Wpymin_ $ymin\n\
set Wpxmax_ $xmax\n\
set Wpymax_ $ymax\n\
}\n\
\n\
\n\
\n\
\n\
set tk_strictMotif 0\n\
\n\
set uscale(m) 1e-3\n\
set uscale(u) 1e-6\n\
set uscale(k) 1e3\n\
set uscale(K) 1e3\n\
set uscale(M) 1e6\n\
\n\
set ANIMATOR_CONTROL_CLASS AnimControl\n\
\n\
Animator set id_ 0\n\
\n\
AnimControl set instance_ 	\"\"\n\
AnimControl set INIT_PORT_ 	20000	;# Arbitrary value\n\
AnimControl set PORT_FILE_ 	\"[lindex [glob ~] 0]/.nam-port\"\n\
;# File which stores server port\n\
\n\
AnimControl set ANIMATOR_CLASS_	Animator\n\
;# Overwrite this in .nam.tcl to use derived animator class\n\
\n\
Animator set welcome \"                               Welcome to NamGraph 1.15                         \"\n\
\n\
\n\
proc nam_init { trace_file args } {\n\
set actrl [new AnimControl $trace_file $args]\n\
\n\
nam_init_hook\n\
}\n\
\n\
proc nam_init_hook {} {\n\
}\n\
\n\
\n\
set defaultRNG [new RNG]\n\
$defaultRNG seed 1\n\
$defaultRNG default\n\
trace variable defaultRNG w { abort \"cannot update defaultRNG once assigned\"; }\n\
\n\
";
#include "tclcl.h"
EmbeddedTcl et_nam(code);
