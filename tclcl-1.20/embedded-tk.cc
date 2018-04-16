static char code[] = "\n\
if {[info commands package] == \"\"} {\n\
error \"version mismatch: library\\nscripts expect Tcl version 7.5b1 or later but the loaded version is\\nonly [info patchlevel]\"\n\
}\n\
package require -exact Tcl 8.5.10\n\
\n\
\n\
if {![info exists auto_path]} {\n\
if {[info exists env(TCLLIBPATH)]} {\n\
set auto_path $env(TCLLIBPATH)\n\
} else {\n\
set auto_path \"\"\n\
}\n\
}\n\
namespace eval tcl {\n\
variable Dir\n\
foreach Dir [list $::tcl_library [file dirname $::tcl_library]] {\n\
if {$Dir ni $::auto_path} {\n\
lappend ::auto_path $Dir\n\
}\n\
}\n\
set Dir [file join [file dirname [file dirname \\\n\
[info nameofexecutable]]] lib]\n\
if {$Dir ni $::auto_path} {\n\
lappend ::auto_path $Dir\n\
}\n\
catch {\n\
foreach Dir $::tcl_pkgPath {\n\
if {$Dir ni $::auto_path} {\n\
lappend ::auto_path $Dir\n\
}\n\
}\n\
}\n\
\n\
if {![interp issafe]} {\n\
variable Path [encoding dirs]\n\
set Dir [file join $::tcl_library encoding]\n\
if {$Dir ni $Path} {\n\
lappend Path $Dir\n\
encoding dirs $Path\n\
}\n\
}\n\
\n\
namespace eval mathfunc {\n\
proc min {args} {\n\
if {[llength $args] == 0} {\n\
return -code error \\\n\
\"too few arguments to math function \\\"min\\\"\"\n\
}\n\
set val Inf\n\
foreach arg $args {\n\
if {[catch {expr {double($arg)}} err]} {\n\
return -code error $err\n\
}\n\
if {$arg < $val} { set val $arg }\n\
}\n\
return $val\n\
}\n\
proc max {args} {\n\
if {[llength $args] == 0} {\n\
return -code error \\\n\
\"too few arguments to math function \\\"max\\\"\"\n\
}\n\
set val -Inf\n\
foreach arg $args {\n\
if {[catch {expr {double($arg)}} err]} {\n\
return -code error $err\n\
}\n\
if {$arg > $val} { set val $arg }\n\
}\n\
return $val\n\
}\n\
namespace export min max\n\
}\n\
}\n\
\n\
\n\
if {(![interp issafe]) && ($tcl_platform(platform) eq \"windows\")} {\n\
namespace eval tcl {\n\
proc EnvTraceProc {lo n1 n2 op} {\n\
set x $::env($n2)\n\
set ::env($lo) $x\n\
set ::env([string toupper $lo]) $x\n\
}\n\
proc InitWinEnv {} {\n\
global env tcl_platform\n\
foreach p [array names env] {\n\
set u [string toupper $p]\n\
if {$u ne $p} {\n\
switch -- $u {\n\
COMSPEC -\n\
PATH {\n\
if {![info exists env($u)]} {\n\
set env($u) $env($p)\n\
}\n\
trace add variable env($p) write \\\n\
[namespace code [list EnvTraceProc $p]]\n\
trace add variable env($u) write \\\n\
[namespace code [list EnvTraceProc $p]]\n\
}\n\
}\n\
}\n\
}\n\
if {![info exists env(COMSPEC)]} {\n\
if {$tcl_platform(os) eq \"Windows NT\"} {\n\
set env(COMSPEC) cmd.exe\n\
} else {\n\
set env(COMSPEC) command.com\n\
}\n\
}\n\
}\n\
InitWinEnv\n\
}\n\
}\n\
\n\
\n\
\n\
if {[interp issafe]} {\n\
package unknown {::tcl::tm::UnknownHandler ::tclPkgUnknown}\n\
} else {\n\
if {$::tcl_platform(os) eq \"Darwin\"\n\
&& $::tcl_platform(platform) eq \"unix\"} {\n\
package unknown {::tcl::tm::UnknownHandler \\\n\
{::tcl::MacOSXPkgUnknown ::tclPkgUnknown}}\n\
} else {\n\
package unknown {::tcl::tm::UnknownHandler ::tclPkgUnknown}\n\
}\n\
\n\
\n\
namespace eval ::tcl::clock [list variable TclLibDir $::tcl_library]\n\
\n\
proc clock args {\n\
namespace eval ::tcl::clock [list namespace ensemble create -command \\\n\
[uplevel 1 [list namespace origin [lindex [info level 0] 0]]] \\\n\
-subcommands {\n\
add clicks format microseconds milliseconds scan seconds\n\
}]\n\
\n\
\n\
foreach cmd {add format scan} {\n\
proc ::tcl::clock::$cmd args {\n\
variable TclLibDir\n\
return [uplevel 1 [info level 0]]\n\
}\n\
}\n\
\n\
return [uplevel 1 [info level 0]]\n\
}\n\
}\n\
\n\
\n\
if {[namespace which -command exec] eq \"\"} {\n\
\n\
\n\
set auto_noexec 1\n\
}\n\
\n\
\n\
if {[namespace which -command tclLog] eq \"\"} {\n\
proc tclLog {string} {\n\
catch {puts stderr $string}\n\
}\n\
}\n\
\n\
\n\
proc unknown args {\n\
variable ::tcl::UnknownPending\n\
global auto_noexec auto_noload env tcl_interactive\n\
\n\
\n\
set cmd [lindex $args 0]\n\
if {[regexp \"^:*namespace\\[ \\t\\n\\]+inscope\" $cmd] && [llength $cmd] == 4} {\n\
set arglist [lrange $args 1 end]\n\
set ret [catch {uplevel 1 ::$cmd $arglist} result opts]\n\
dict unset opts -errorinfo\n\
dict incr opts -level\n\
return -options $opts $result\n\
}\n\
\n\
catch {set savedErrorInfo $::errorInfo}\n\
catch {set savedErrorCode $::errorCode}\n\
set name $cmd\n\
if {![info exists auto_noload]} {\n\
if {[info exists UnknownPending($name)]} {\n\
return -code error \"self-referential recursion\\\n\
in \\\"unknown\\\" for command \\\"$name\\\"\";\n\
}\n\
set UnknownPending($name) pending;\n\
set ret [catch {\n\
auto_load $name [uplevel 1 {::namespace current}]\n\
} msg opts]\n\
unset UnknownPending($name);\n\
if {$ret != 0} {\n\
dict append opts -errorinfo \"\\n    (autoloading \\\"$name\\\")\"\n\
return -options $opts $msg\n\
}\n\
if {![array size UnknownPending]} {\n\
unset UnknownPending\n\
}\n\
if {$msg} {\n\
if {[info exists savedErrorCode]} {\n\
set ::errorCode $savedErrorCode\n\
} else {\n\
unset -nocomplain ::errorCode\n\
}\n\
if {[info exists savedErrorInfo]} {\n\
set ::errorInfo $savedErrorInfo\n\
} else {\n\
unset -nocomplain ::errorInfo\n\
}\n\
set code [catch {uplevel 1 $args} msg opts]\n\
if {$code ==  1} {\n\
set errorInfo [dict get $opts -errorinfo]\n\
set errorCode [dict get $opts -errorcode]\n\
set cinfo $args\n\
if {[string bytelength $cinfo] > 150} {\n\
set cinfo [string range $cinfo 0 150]\n\
while {[string bytelength $cinfo] > 150} {\n\
set cinfo [string range $cinfo 0 end-1]\n\
}\n\
append cinfo ...\n\
}\n\
append cinfo \"\\\"\\n    (\\\"uplevel\\\" body line 1)\"\n\
append cinfo \"\\n    invoked from within\"\n\
append cinfo \"\\n\\\"uplevel 1 \\$args\\\"\"\n\
set expect \"$msg\\n    while executing\\n\\\"$cinfo\"\n\
if {$errorInfo eq $expect} {\n\
dict unset opts -errorinfo\n\
dict incr opts -level\n\
return -options $opts $msg\n\
}\n\
set expect \"\\n    invoked from within\\n\\\"$cinfo\"\n\
set exlen [string length $expect]\n\
set eilen [string length $errorInfo]\n\
set i [expr {$eilen - $exlen - 1}]\n\
set einfo [string range $errorInfo 0 $i]\n\
if {$errorInfo ne \"$einfo$expect\"} {\n\
error \"Tcl bug: unexpected stack trace in \\\"unknown\\\"\" {} \\\n\
[list CORE UNKNOWN BADTRACE $einfo $expect $errorInfo]\n\
}\n\
return -code error -errorcode $errorCode \\\n\
-errorinfo $einfo $msg\n\
} else {\n\
dict incr opts -level\n\
return -options $opts $msg\n\
}\n\
}\n\
}\n\
\n\
if {([info level] == 1) && ([info script] eq \"\") \\\n\
&& [info exists tcl_interactive] && $tcl_interactive} {\n\
if {![info exists auto_noexec]} {\n\
set new [auto_execok $name]\n\
if {$new ne \"\"} {\n\
set redir \"\"\n\
if {[namespace which -command console] eq \"\"} {\n\
set redir \">&@stdout <@stdin\"\n\
}\n\
uplevel 1 [list ::catch \\\n\
[concat exec $redir $new [lrange $args 1 end]] \\\n\
::tcl::UnknownResult ::tcl::UnknownOptions]\n\
dict incr ::tcl::UnknownOptions -level\n\
return -options $::tcl::UnknownOptions $::tcl::UnknownResult\n\
}\n\
}\n\
if {$name eq \"!!\"} {\n\
set newcmd [history event]\n\
} elseif {[regexp {^!(.+)$} $name -> event]} {\n\
set newcmd [history event $event]\n\
} elseif {[regexp {^\\^([^^]*)\\^([^^]*)\\^?$} $name -> old new]} {\n\
set newcmd [history event -1]\n\
catch {regsub -all -- $old $newcmd $new newcmd}\n\
}\n\
if {[info exists newcmd]} {\n\
tclLog $newcmd\n\
history change $newcmd 0\n\
uplevel 1 [list ::catch $newcmd \\\n\
::tcl::UnknownResult ::tcl::UnknownOptions]\n\
dict incr ::tcl::UnknownOptions -level\n\
return -options $::tcl::UnknownOptions $::tcl::UnknownResult\n\
}\n\
\n\
set ret [catch {set candidates [info commands $name*]} msg]\n\
if {$name eq \"::\"} {\n\
set name \"\"\n\
}\n\
if {$ret != 0} {\n\
dict append opts -errorinfo \\\n\
\"\\n    (expanding command prefix \\\"$name\\\" in unknown)\"\n\
return -options $opts $msg\n\
}\n\
if {$name eq \"\"} {\n\
set cmds $candidates\n\
} else {\n\
set cmds [list]\n\
foreach x $candidates {\n\
if {[string first $name $x] == 0} {\n\
lappend cmds $x\n\
}\n\
}\n\
}\n\
if {[llength $cmds] == 1} {\n\
uplevel 1 [list ::catch [lreplace $args 0 0 [lindex $cmds 0]] \\\n\
::tcl::UnknownResult ::tcl::UnknownOptions]\n\
dict incr ::tcl::UnknownOptions -level\n\
return -options $::tcl::UnknownOptions $::tcl::UnknownResult\n\
}\n\
if {[llength $cmds]} {\n\
return -code error \"ambiguous command name \\\"$name\\\": [lsort $cmds]\"\n\
}\n\
}\n\
return -code error \"invalid command name \\\"$name\\\"\"\n\
}\n\
\n\
\n\
proc auto_load {cmd {namespace {}}} {\n\
global auto_index auto_path\n\
\n\
if {$namespace eq \"\"} {\n\
set namespace [uplevel 1 [list ::namespace current]]\n\
}\n\
set nameList [auto_qualify $cmd $namespace]\n\
lappend nameList $cmd\n\
foreach name $nameList {\n\
if {[info exists auto_index($name)]} {\n\
namespace eval :: $auto_index($name)\n\
if {[namespace which -command $name] ne \"\"} {\n\
return 1\n\
}\n\
}\n\
}\n\
if {![info exists auto_path]} {\n\
return 0\n\
}\n\
\n\
if {![auto_load_index]} {\n\
return 0\n\
}\n\
foreach name $nameList {\n\
if {[info exists auto_index($name)]} {\n\
namespace eval :: $auto_index($name)\n\
if {[namespace which -command $name] ne \"\"} {\n\
return 1\n\
}\n\
}\n\
}\n\
return 0\n\
}\n\
\n\
\n\
proc auto_load_index {} {\n\
variable ::tcl::auto_oldpath\n\
global auto_index auto_path\n\
\n\
if {[info exists auto_oldpath] && ($auto_oldpath eq $auto_path)} {\n\
return 0\n\
}\n\
set auto_oldpath $auto_path\n\
\n\
\n\
set issafe [interp issafe]\n\
for {set i [expr {[llength $auto_path] - 1}]} {$i >= 0} {incr i -1} {\n\
set dir [lindex $auto_path $i]\n\
set f \"\"\n\
if {$issafe} {\n\
catch {source [file join $dir tclIndex]}\n\
} elseif {[catch {set f [open [file join $dir tclIndex]]}]} {\n\
continue\n\
} else {\n\
set error [catch {\n\
set id [gets $f]\n\
if {$id eq \"# Tcl autoload index file, version 2.0\"} {\n\
eval [read $f]\n\
} elseif {$id eq \"# Tcl autoload index file: each line identifies a Tcl\"} {\n\
while {[gets $f line] >= 0} {\n\
if {([string index $line 0] eq \"#\") \\\n\
|| ([llength $line] != 2)} {\n\
continue\n\
}\n\
set name [lindex $line 0]\n\
set auto_index($name) \\\n\
\"source [file join $dir [lindex $line 1]]\"\n\
}\n\
} else {\n\
error \"[file join $dir tclIndex] isn't a proper Tcl index file\"\n\
}\n\
} msg opts]\n\
if {$f ne \"\"} {\n\
close $f\n\
}\n\
if {$error} {\n\
return -options $opts $msg\n\
}\n\
}\n\
}\n\
return 1\n\
}\n\
\n\
\n\
proc auto_qualify {cmd namespace} {\n\
\n\
set n [regsub -all {::+} $cmd :: cmd]\n\
\n\
\n\
\n\
if {[string match ::* $cmd]} {\n\
if {$n > 1} {\n\
return [list $cmd]\n\
} else {\n\
return [list [string range $cmd 2 end]]\n\
}\n\
}\n\
\n\
\n\
if {$n == 0} {\n\
if {$namespace eq \"::\"} {\n\
return [list $cmd]\n\
} else {\n\
return [list ${namespace}::$cmd $cmd]\n\
}\n\
} elseif {$namespace eq \"::\"} {\n\
return [list ::$cmd]\n\
} else {\n\
return [list ${namespace}::$cmd ::$cmd]\n\
}\n\
}\n\
\n\
\n\
proc auto_import {pattern} {\n\
global auto_index\n\
\n\
\n\
if {![string match *::* $pattern]} {\n\
return\n\
}\n\
\n\
set ns [uplevel 1 [list ::namespace current]]\n\
set patternList [auto_qualify $pattern $ns]\n\
\n\
auto_load_index\n\
\n\
foreach pattern $patternList {\n\
foreach name [array names auto_index $pattern] {\n\
if {([namespace which -command $name] eq \"\")\n\
&& ([namespace qualifiers $pattern] eq [namespace qualifiers $name])} {\n\
namespace eval :: $auto_index($name)\n\
}\n\
}\n\
}\n\
}\n\
\n\
\n\
if {$tcl_platform(platform) eq \"windows\"} {\n\
proc auto_execok name {\n\
global auto_execs env tcl_platform\n\
\n\
if {[info exists auto_execs($name)]} {\n\
return $auto_execs($name)\n\
}\n\
set auto_execs($name) \"\"\n\
\n\
set shellBuiltins [list cls copy date del erase dir echo mkdir \\\n\
md rename ren rmdir rd time type ver vol]\n\
if {$tcl_platform(os) eq \"Windows NT\"} {\n\
lappend shellBuiltins \"start\"\n\
}\n\
if {[info exists env(PATHEXT)]} {\n\
set execExtensions [split \";$env(PATHEXT)\" \";\"]\n\
} else {\n\
set execExtensions [list {} .com .exe .bat .cmd]\n\
}\n\
\n\
if {$name in $shellBuiltins} {\n\
set cmd $env(COMSPEC)\n\
if {[file exists $cmd]} {\n\
set cmd [file attributes $cmd -shortname]\n\
}\n\
return [set auto_execs($name) [list $cmd /c $name]]\n\
}\n\
\n\
if {[llength [file split $name]] != 1} {\n\
foreach ext $execExtensions {\n\
set file ${name}${ext}\n\
if {[file exists $file] && ![file isdirectory $file]} {\n\
return [set auto_execs($name) [list $file]]\n\
}\n\
}\n\
return \"\"\n\
}\n\
\n\
set path \"[file dirname [info nameof]];.;\"\n\
if {[info exists env(WINDIR)]} {\n\
set windir $env(WINDIR) \n\
}\n\
if {[info exists windir]} {\n\
if {$tcl_platform(os) eq \"Windows NT\"} {\n\
append path \"$windir/system32;\"\n\
}\n\
append path \"$windir/system;$windir;\"\n\
}\n\
\n\
foreach var {PATH Path path} {\n\
if {[info exists env($var)]} {\n\
append path \";$env($var)\"\n\
}\n\
}\n\
\n\
foreach dir [split $path {;}] {\n\
if {[info exists checked($dir)] || ($dir eq {})} { continue }\n\
set checked($dir) {}\n\
foreach ext $execExtensions {\n\
set file [file join $dir ${name}${ext}]\n\
if {[file exists $file] && ![file isdirectory $file]} {\n\
return [set auto_execs($name) [list $file]]\n\
}\n\
}\n\
}\n\
return \"\"\n\
}\n\
\n\
} else {\n\
proc auto_execok name {\n\
global auto_execs env\n\
\n\
if {[info exists auto_execs($name)]} {\n\
return $auto_execs($name)\n\
}\n\
set auto_execs($name) \"\"\n\
if {[llength [file split $name]] != 1} {\n\
if {[file executable $name] && ![file isdirectory $name]} {\n\
set auto_execs($name) [list $name]\n\
}\n\
return $auto_execs($name)\n\
}\n\
foreach dir [split $env(PATH) :] {\n\
if {$dir eq \"\"} {\n\
set dir .\n\
}\n\
set file [file join $dir $name]\n\
if {[file executable $file] && ![file isdirectory $file]} {\n\
set auto_execs($name) [list $file]\n\
return $auto_execs($name)\n\
}\n\
}\n\
return \"\"\n\
}\n\
\n\
}\n\
\n\
proc tcl::CopyDirectory {action src dest} {\n\
set nsrc [file normalize $src]\n\
set ndest [file normalize $dest]\n\
\n\
if {$action eq \"renaming\"} {\n\
if {$nsrc in [file volumes]} {\n\
return -code error \"error $action \\\"$src\\\" to\\\n\
\\\"$dest\\\": trying to rename a volume or move a directory\\\n\
into itself\"\n\
}\n\
}\n\
if {[file exists $dest]} {\n\
if {$nsrc eq $ndest} {\n\
return -code error \"error $action \\\"$src\\\" to\\\n\
\\\"$dest\\\": trying to rename a volume or move a directory\\\n\
into itself\"\n\
}\n\
if {$action eq \"copying\"} {\n\
} else {\n\
set existing [glob -nocomplain -directory $dest * .*]\n\
lappend existing {*}[glob -nocomplain -directory $dest \\\n\
-type hidden * .*]\n\
foreach s $existing {\n\
if {([file tail $s] ne \".\") && ([file tail $s] ne \"..\")} {\n\
return -code error \"error $action \\\"$src\\\" to\\\n\
\\\"$dest\\\": file already exists\"\n\
}\n\
}\n\
}\n\
} else {\n\
if {[string first $nsrc $ndest] != -1} {\n\
set srclen [expr {[llength [file split $nsrc]] -1}]\n\
set ndest [lindex [file split $ndest] $srclen]\n\
if {$ndest eq [file tail $nsrc]} {\n\
return -code error \"error $action \\\"$src\\\" to\\\n\
\\\"$dest\\\": trying to rename a volume or move a directory\\\n\
into itself\"\n\
}\n\
}\n\
file mkdir $dest\n\
}\n\
set filelist [concat [glob -nocomplain -directory $src *] \\\n\
[glob -nocomplain -directory $src -types hidden *]]\n\
\n\
foreach s [lsort -unique $filelist] {\n\
if {([file tail $s] ne \".\") && ([file tail $s] ne \"..\")} {\n\
file copy -force $s [file join $dest [file tail $s]]\n\
}\n\
}\n\
return\n\
}\n\
\n\
\n\
namespace eval tcl {\n\
variable history\n\
if {![info exists history]} {\n\
array set history {\n\
nextid	0\n\
keep	20\n\
oldest	-20\n\
}\n\
}\n\
}\n\
\n\
\n\
proc history {args} {\n\
set len [llength $args]\n\
if {$len == 0} {\n\
return [tcl::HistInfo]\n\
}\n\
set key [lindex $args 0]\n\
set options \"add, change, clear, event, info, keep, nextid, or redo\"\n\
switch -glob -- $key {\n\
a* { # history add\n\
\n\
if {$len > 3} {\n\
return -code error \"wrong # args: should be \\\"history add event ?exec?\\\"\"\n\
}\n\
if {![string match $key* add]} {\n\
return -code error \"bad option \\\"$key\\\": must be $options\"\n\
}\n\
if {$len == 3} {\n\
set arg [lindex $args 2]\n\
if {! ([string match e* $arg] && [string match $arg* exec])} {\n\
return -code error \"bad argument \\\"$arg\\\": should be \\\"exec\\\"\"\n\
}\n\
}\n\
return [tcl::HistAdd [lindex $args 1] [lindex $args 2]]\n\
}\n\
ch* { # history change\n\
\n\
if {($len > 3) || ($len < 2)} {\n\
return -code error \"wrong # args: should be \\\"history change newValue ?event?\\\"\"\n\
}\n\
if {![string match $key* change]} {\n\
return -code error \"bad option \\\"$key\\\": must be $options\"\n\
}\n\
if {$len == 2} {\n\
set event 0\n\
} else {\n\
set event [lindex $args 2]\n\
}\n\
\n\
return [tcl::HistChange [lindex $args 1] $event]\n\
}\n\
cl* { # history clear\n\
\n\
if {($len > 1)} {\n\
return -code error \"wrong # args: should be \\\"history clear\\\"\"\n\
}\n\
if {![string match $key* clear]} {\n\
return -code error \"bad option \\\"$key\\\": must be $options\"\n\
}\n\
return [tcl::HistClear]\n\
}\n\
e* { # history event\n\
\n\
if {$len > 2} {\n\
return -code error \"wrong # args: should be \\\"history event ?event?\\\"\"\n\
}\n\
if {![string match $key* event]} {\n\
return -code error \"bad option \\\"$key\\\": must be $options\"\n\
}\n\
if {$len == 1} {\n\
set event -1\n\
} else {\n\
set event [lindex $args 1]\n\
}\n\
return [tcl::HistEvent $event]\n\
}\n\
i* { # history info\n\
\n\
if {$len > 2} {\n\
return -code error \"wrong # args: should be \\\"history info ?count?\\\"\"\n\
}\n\
if {![string match $key* info]} {\n\
return -code error \"bad option \\\"$key\\\": must be $options\"\n\
}\n\
return [tcl::HistInfo [lindex $args 1]]\n\
}\n\
k* { # history keep\n\
\n\
if {$len > 2} {\n\
return -code error \"wrong # args: should be \\\"history keep ?count?\\\"\"\n\
}\n\
if {$len == 1} {\n\
return [tcl::HistKeep]\n\
} else {\n\
set limit [lindex $args 1]\n\
if {[catch {expr {~$limit}}] || ($limit < 0)} {\n\
return -code error \"illegal keep count \\\"$limit\\\"\"\n\
}\n\
return [tcl::HistKeep $limit]\n\
}\n\
}\n\
n* { # history nextid\n\
\n\
if {$len > 1} {\n\
return -code error \"wrong # args: should be \\\"history nextid\\\"\"\n\
}\n\
if {![string match $key* nextid]} {\n\
return -code error \"bad option \\\"$key\\\": must be $options\"\n\
}\n\
return [expr {$tcl::history(nextid) + 1}]\n\
}\n\
r* { # history redo\n\
\n\
if {$len > 2} {\n\
return -code error \"wrong # args: should be \\\"history redo ?event?\\\"\"\n\
}\n\
if {![string match $key* redo]} {\n\
return -code error \"bad option \\\"$key\\\": must be $options\"\n\
}\n\
return [tcl::HistRedo [lindex $args 1]]\n\
}\n\
default {\n\
return -code error \"bad option \\\"$key\\\": must be $options\"\n\
}\n\
}\n\
}\n\
\n\
\n\
proc tcl::HistAdd {command {exec {}}} {\n\
variable history\n\
\n\
if {[string trim $command] eq \"\"} {\n\
return \"\"\n\
}\n\
\n\
set i [incr history(nextid)]\n\
set history($i) $command\n\
set j [incr history(oldest)]\n\
unset -nocomplain history($j)\n\
if {[string match e* $exec]} {\n\
return [uplevel #0 $command]\n\
} else {\n\
return {}\n\
}\n\
}\n\
\n\
\n\
proc tcl::HistKeep {{limit {}}} {\n\
variable history\n\
if {$limit eq \"\"} {\n\
return $history(keep)\n\
} else {\n\
set oldold $history(oldest)\n\
set history(oldest) [expr {$history(nextid) - $limit}]\n\
for {} {$oldold <= $history(oldest)} {incr oldold} {\n\
unset -nocomplain history($oldold)\n\
}\n\
set history(keep) $limit\n\
}\n\
}\n\
\n\
\n\
proc tcl::HistClear {} {\n\
variable history\n\
set keep $history(keep)\n\
unset history\n\
array set history [list \\\n\
nextid	0	\\\n\
keep	$keep	\\\n\
oldest	-$keep	\\\n\
]\n\
}\n\
\n\
\n\
proc tcl::HistInfo {{num {}}} {\n\
variable history\n\
if {$num eq \"\"} {\n\
set num [expr {$history(keep) + 1}]\n\
}\n\
set result {}\n\
set newline \"\"\n\
for {set i [expr {$history(nextid) - $num + 1}]} \\\n\
{$i <= $history(nextid)} {incr i} {\n\
if {![info exists history($i)]} {\n\
continue\n\
}\n\
set cmd [string map [list \\n \\n\\t] [string trimright $history($i) \\ \\n]]\n\
append result $newline[format \"%6d  %s\" $i $cmd]\n\
set newline \\n\n\
}\n\
return $result\n\
}\n\
\n\
\n\
proc tcl::HistRedo {{event -1}} {\n\
variable history\n\
if {$event eq \"\"} {\n\
set event -1\n\
}\n\
set i [HistIndex $event]\n\
if {$i == $history(nextid)} {\n\
return -code error \"cannot redo the current event\"\n\
}\n\
set cmd $history($i)\n\
HistChange $cmd 0\n\
uplevel #0 $cmd\n\
}\n\
\n\
\n\
proc tcl::HistIndex {event} {\n\
variable history\n\
if {[catch {expr {~$event}}]} {\n\
for {set i [expr {$history(nextid)-1}]} {[info exists history($i)]} \\\n\
{incr i -1} {\n\
if {[string match $event* $history($i)]} {\n\
return $i;\n\
}\n\
if {[string match $event $history($i)]} {\n\
return $i;\n\
}\n\
}\n\
return -code error \"no event matches \\\"$event\\\"\"\n\
} elseif {$event <= 0} {\n\
set i [expr {$history(nextid) + $event}]\n\
} else {\n\
set i $event\n\
}\n\
if {$i <= $history(oldest)} {\n\
return -code error \"event \\\"$event\\\" is too far in the past\"\n\
}\n\
if {$i > $history(nextid)} {\n\
return -code error \"event \\\"$event\\\" hasn't occured yet\"\n\
}\n\
return $i\n\
}\n\
\n\
\n\
proc tcl::HistEvent {event} {\n\
variable history\n\
set i [HistIndex $event]\n\
if {[info exists history($i)]} {\n\
return [string trimright $history($i) \\ \\n]\n\
} else {\n\
return \"\";\n\
}\n\
}\n\
\n\
\n\
proc tcl::HistChange {cmd {event 0}} {\n\
variable history\n\
set i [HistIndex $event]\n\
set history($i) $cmd\n\
}\n\
\n\
\n\
if {$::tcl_platform(platform) eq \"windows\"} {\n\
set ::tcl_wordchars {\\S}\n\
set ::tcl_nonwordchars {\\s}\n\
} else {\n\
set ::tcl_wordchars {\\w}\n\
set ::tcl_nonwordchars {\\W}\n\
}\n\
\n\
\n\
namespace eval ::tcl {\n\
variable WordBreakRE\n\
array set WordBreakRE {}\n\
\n\
proc UpdateWordBreakREs args {\n\
global tcl_wordchars tcl_nonwordchars\n\
variable WordBreakRE\n\
\n\
set letter $tcl_wordchars\n\
set space $tcl_nonwordchars\n\
\n\
set WordBreakRE(after)		\"$letter$space|$space$letter\"\n\
set WordBreakRE(before)		\"^.*($letter$space|$space$letter)\"\n\
set WordBreakRE(end)		\"$space*$letter+$space\"\n\
set WordBreakRE(next)		\"$letter*$space+$letter\"\n\
set WordBreakRE(previous)	\"$space*($letter+)$space*\\$\"\n\
}\n\
\n\
UpdateWordBreakREs\n\
trace add variable ::tcl_wordchars write ::tcl::UpdateWordBreakREs\n\
trace add variable ::tcl_nonwordchars write ::tcl::UpdateWordBreakREs\n\
}\n\
\n\
\n\
proc tcl_wordBreakAfter {str start} {\n\
variable ::tcl::WordBreakRE\n\
set result {-1 -1}\n\
regexp -indices -start $start $WordBreakRE(after) $str result\n\
return [lindex $result 1]\n\
}\n\
\n\
\n\
proc tcl_wordBreakBefore {str start} {\n\
variable ::tcl::WordBreakRE\n\
set result {-1 -1}\n\
regexp -indices $WordBreakRE(before) [string range $str 0 $start] result\n\
return [lindex $result 1]\n\
}\n\
\n\
\n\
proc tcl_endOfWord {str start} {\n\
variable ::tcl::WordBreakRE\n\
set result {-1 -1}\n\
regexp -indices -start $start $WordBreakRE(end) $str result\n\
return [lindex $result 1]\n\
}\n\
\n\
\n\
proc tcl_startOfNextWord {str start} {\n\
variable ::tcl::WordBreakRE\n\
set result {-1 -1}\n\
regexp -indices -start $start $WordBreakRE(next) $str result\n\
return [lindex $result 1]\n\
}\n\
\n\
\n\
proc tcl_startOfPreviousWord {str start} {\n\
variable ::tcl::WordBreakRE\n\
set word {-1 -1}\n\
regexp -indices $WordBreakRE(previous) [string range $str 0 $start-1] \\\n\
result word\n\
return [lindex $word 0]\n\
}\n\
\n\
package provide http 1.0\n\
\n\
array set http {\n\
-accept */*\n\
-proxyhost {}\n\
-proxyport {}\n\
-useragent {Tcl http client package 1.0}\n\
-proxyfilter httpProxyRequired\n\
}\n\
proc http_config {args} {\n\
global http\n\
set options [lsort [array names http -*]]\n\
set usage [join $options \", \"]\n\
if {[llength $args] == 0} {\n\
set result {}\n\
foreach name $options {\n\
lappend result $name $http($name)\n\
}\n\
return $result\n\
}\n\
regsub -all -- - $options {} options\n\
set pat ^-([join $options |])$\n\
if {[llength $args] == 1} {\n\
set flag [lindex $args 0]\n\
if {[regexp -- $pat $flag]} {\n\
return $http($flag)\n\
} else {\n\
return -code error \"Unknown option $flag, must be: $usage\"\n\
}\n\
} else {\n\
foreach {flag value} $args {\n\
if {[regexp -- $pat $flag]} {\n\
set http($flag) $value\n\
} else {\n\
return -code error \"Unknown option $flag, must be: $usage\"\n\
}\n\
}\n\
}\n\
}\n\
\n\
proc httpFinish { token {errormsg \"\"} } {\n\
upvar #0 $token state\n\
global errorInfo errorCode\n\
if {[string length $errormsg] != 0} {\n\
set state(error) [list $errormsg $errorInfo $errorCode]\n\
set state(status) error\n\
}\n\
catch {close $state(sock)}\n\
catch {after cancel $state(after)}\n\
if {[info exists state(-command)]} {\n\
if {[catch {eval $state(-command) {$token}} err]} {\n\
if {[string length $errormsg] == 0} {\n\
set state(error) [list $err $errorInfo $errorCode]\n\
set state(status) error\n\
}\n\
}\n\
unset state(-command)\n\
}\n\
}\n\
proc http_reset { token {why reset} } {\n\
upvar #0 $token state\n\
set state(status) $why\n\
catch {fileevent $state(sock) readable {}}\n\
httpFinish $token\n\
if {[info exists state(error)]} {\n\
set errorlist $state(error)\n\
unset state(error)\n\
eval error $errorlist\n\
}\n\
}\n\
proc http_get { url args } {\n\
global http\n\
if {![info exists http(uid)]} {\n\
set http(uid) 0\n\
}\n\
set token http#[incr http(uid)]\n\
upvar #0 $token state\n\
http_reset $token\n\
array set state {\n\
-blocksize 	8192\n\
-validate 	0\n\
-headers 	{}\n\
-timeout 	0\n\
state		header\n\
meta		{}\n\
currentsize	0\n\
totalsize	0\n\
type            text/html\n\
body            {}\n\
status		\"\"\n\
}\n\
set options {-blocksize -channel -command -handler -headers \\\n\
-progress -query -validate -timeout}\n\
set usage [join $options \", \"]\n\
regsub -all -- - $options {} options\n\
set pat ^-([join $options |])$\n\
foreach {flag value} $args {\n\
if {[regexp $pat $flag]} {\n\
if {[info exists state($flag)] && \\\n\
[regexp {^[0-9]+$} $state($flag)] && \\\n\
![regexp {^[0-9]+$} $value]} {\n\
return -code error \"Bad value for $flag ($value), must be integer\"\n\
}\n\
set state($flag) $value\n\
} else {\n\
return -code error \"Unknown option $flag, can be: $usage\"\n\
}\n\
}\n\
if {! [regexp -nocase {^(http://)?([^/:]+)(:([0-9]+))?(/.*)?$} $url \\\n\
x proto host y port srvurl]} {\n\
error \"Unsupported URL: $url\"\n\
}\n\
if {[string length $port] == 0} {\n\
set port 80\n\
}\n\
if {[string length $srvurl] == 0} {\n\
set srvurl /\n\
}\n\
if {[string length $proto] == 0} {\n\
set url http://$url\n\
}\n\
set state(url) $url\n\
if {![catch {$http(-proxyfilter) $host} proxy]} {\n\
set phost [lindex $proxy 0]\n\
set pport [lindex $proxy 1]\n\
}\n\
if {$state(-timeout) > 0} {\n\
set state(after) [after $state(-timeout) [list http_reset $token timeout]]\n\
}\n\
if {[info exists phost] && [string length $phost]} {\n\
set srvurl $url\n\
set s [socket $phost $pport]\n\
} else {\n\
set s [socket $host $port]\n\
}\n\
set state(sock) $s\n\
\n\
\n\
fconfigure $s -translation {auto crlf} -buffersize $state(-blocksize)\n\
\n\
\n\
catch {fconfigure $s -blocking off}\n\
set len 0\n\
set how GET\n\
if {[info exists state(-query)]} {\n\
set len [string length $state(-query)]\n\
if {$len > 0} {\n\
set how POST\n\
}\n\
} elseif {$state(-validate)} {\n\
set how HEAD\n\
}\n\
puts $s \"$how $srvurl HTTP/1.0\"\n\
puts $s \"Accept: $http(-accept)\"\n\
puts $s \"Host: $host\"\n\
puts $s \"User-Agent: $http(-useragent)\"\n\
foreach {key value} $state(-headers) {\n\
regsub -all \\[\\n\\r\\]  $value {} value\n\
set key [string trim $key]\n\
if {[string length $key]} {\n\
puts $s \"$key: $value\"\n\
}\n\
}\n\
if {$len > 0} {\n\
puts $s \"Content-Length: $len\"\n\
puts $s \"Content-Type: application/x-www-form-urlencoded\"\n\
puts $s \"\"\n\
fconfigure $s -translation {auto binary}\n\
puts -nonewline $s $state(-query)\n\
} else {\n\
puts $s \"\"\n\
}\n\
flush $s\n\
fileevent $s readable [list httpEvent $token]\n\
if {! [info exists state(-command)]} {\n\
http_wait $token\n\
}\n\
return $token\n\
}\n\
proc http_data {token} {\n\
upvar #0 $token state\n\
return $state(body)\n\
}\n\
proc http_status {token} {\n\
upvar #0 $token state\n\
return $state(status)\n\
}\n\
proc http_code {token} {\n\
upvar #0 $token state\n\
return $state(http)\n\
}\n\
proc http_size {token} {\n\
upvar #0 $token state\n\
return $state(currentsize)\n\
}\n\
\n\
proc httpEvent {token} {\n\
upvar #0 $token state\n\
set s $state(sock)\n\
\n\
if {[eof $s]} {\n\
httpEof $token\n\
return\n\
}\n\
if {$state(state) == \"header\"} {\n\
set n [gets $s line]\n\
if {$n == 0} {\n\
set state(state) body\n\
if {![regexp -nocase ^text $state(type)]} {\n\
fconfigure $s -translation binary\n\
if {[info exists state(-channel)]} {\n\
fconfigure $state(-channel) -translation binary\n\
}\n\
}\n\
if {[info exists state(-channel)] &&\n\
![info exists state(-handler)]} {\n\
fileevent $s readable {}\n\
httpCopyStart $s $token\n\
}\n\
} elseif {$n > 0} {\n\
if {[regexp -nocase {^content-type:(.+)$} $line x type]} {\n\
set state(type) [string trim $type]\n\
}\n\
if {[regexp -nocase {^content-length:(.+)$} $line x length]} {\n\
set state(totalsize) [string trim $length]\n\
}\n\
if {[regexp -nocase {^([^:]+):(.+)$} $line x key value]} {\n\
lappend state(meta) $key $value\n\
} elseif {[regexp ^HTTP $line]} {\n\
set state(http) $line\n\
}\n\
}\n\
} else {\n\
if {[catch {\n\
if {[info exists state(-handler)]} {\n\
set n [eval $state(-handler) {$s $token}]\n\
} else {\n\
set block [read $s $state(-blocksize)]\n\
set n [string length $block]\n\
if {$n >= 0} {\n\
append state(body) $block\n\
}\n\
}\n\
if {$n >= 0} {\n\
incr state(currentsize) $n\n\
}\n\
} err]} {\n\
httpFinish $token $err\n\
} else {\n\
if {[info exists state(-progress)]} {\n\
eval $state(-progress) {$token $state(totalsize) $state(currentsize)}\n\
}\n\
}\n\
}\n\
}\n\
proc httpCopyStart {s token} {\n\
upvar #0 $token state\n\
if {[catch {\n\
fcopy $s $state(-channel) -size $state(-blocksize) -command \\\n\
[list httpCopyDone $token]\n\
} err]} {\n\
httpFinish $token $err\n\
}\n\
}\n\
proc httpCopyDone {token count {error {}}} {\n\
upvar #0 $token state\n\
set s $state(sock)\n\
incr state(currentsize) $count\n\
if {[info exists state(-progress)]} {\n\
eval $state(-progress) {$token $state(totalsize) $state(currentsize)}\n\
}\n\
if {([string length $error] != 0)} {\n\
httpFinish $token $error\n\
} elseif {[eof $s]} {\n\
httpEof $token\n\
} else {\n\
httpCopyStart $s $token\n\
}\n\
}\n\
proc httpEof {token} {\n\
upvar #0 $token state\n\
if {$state(state) == \"header\"} {\n\
set state(status) eof\n\
} else {\n\
set state(status) ok\n\
}\n\
set state(state) eof\n\
httpFinish $token\n\
}\n\
proc http_wait {token} {\n\
upvar #0 $token state\n\
if {![info exists state(status)] || [string length $state(status)] == 0} {\n\
vwait $token\\(status)\n\
}\n\
if {[info exists state(error)]} {\n\
set errorlist $state(error)\n\
unset state(error)\n\
eval error $errorlist\n\
}\n\
return $state(status)\n\
}\n\
\n\
\n\
proc http_formatQuery {args} {\n\
set result \"\"\n\
set sep \"\"\n\
foreach i $args {\n\
append result  $sep [httpMapReply $i]\n\
if {$sep != \"=\"} {\n\
set sep =\n\
} else {\n\
set sep &\n\
}\n\
}\n\
return $result\n\
}\n\
\n\
\n\
proc httpMapReply {string} {\n\
global httpFormMap\n\
set alphanumeric	a-zA-Z0-9\n\
if {![info exists httpFormMap]} {\n\
\n\
for {set i 1} {$i <= 256} {incr i} {\n\
set c [format %c $i]\n\
if {![string match \\[$alphanumeric\\] $c]} {\n\
set httpFormMap($c) %[format %.2x $i]\n\
}\n\
}\n\
array set httpFormMap {\n\
\" \" +   \\n %0d%0a\n\
}\n\
}\n\
regsub -all \\[^$alphanumeric\\] $string {$httpFormMap(&)} string\n\
regsub -all \\n $string {\\\\n} string\n\
regsub -all \\t $string {\\\\t} string\n\
regsub -all {[][{})\\\\]\\)} $string {\\\\&} string\n\
return [subst $string]\n\
}\n\
\n\
proc httpProxyRequired {host} {\n\
global http\n\
if {[info exists http(-proxyhost)] && [string length $http(-proxyhost)]} {\n\
if {![info exists http(-proxyport)] || ![string length $http(-proxyport)]} {\n\
set http(-proxyport) 8080\n\
}\n\
return [list $http(-proxyhost) $http(-proxyport)]\n\
} else {\n\
return {}\n\
}\n\
}\n\
\n\
package require Tcl 8.5	;# Guard against [source] in an 8.4- interp before\n\
;# using 8.5 [package] features.\n\
package require Tcl 8.5.0\n\
package require -exact Tk  8.5.10\n\
\n\
namespace eval ::tk {\n\
namespace eval msgcat {\n\
namespace export mc mcmax\n\
if {[interp issafe] || [catch {package require msgcat}]} {\n\
proc mc {src args} {\n\
return [format $src {*}$args]\n\
}\n\
proc mcmax {args} {\n\
set max 0\n\
foreach string $args {\n\
set len [string length $string]\n\
if {$len>$max} {\n\
set max $len\n\
}\n\
}\n\
return $max\n\
}\n\
} else {\n\
namespace import ::msgcat::mc\n\
namespace import ::msgcat::mcmax\n\
::msgcat::mcload [file join $::tk_library msgs]\n\
}\n\
}\n\
namespace import ::tk::msgcat::*\n\
}\n\
namespace eval ::ttk {\n\
if {$::tk_library ne \"\"} {\n\
variable library $::tk_library/ttk\n\
}\n\
}\n\
\n\
\n\
if {[info exists ::auto_path] && ($::tk_library ne \"\")\n\
&& ($::tk_library ni $::auto_path)} {\n\
lappend ::auto_path $::tk_library $::ttk::library\n\
}\n\
\n\
\n\
set ::tk_strictMotif 0\n\
\n\
\n\
catch {tk useinputmethods 1}\n\
\n\
proc ::tk::PlaceWindow {w {place \"\"} {anchor \"\"}} {\n\
wm withdraw $w\n\
update idletasks\n\
set checkBounds 1\n\
if {$place eq \"\"} {\n\
set x [expr {([winfo screenwidth $w]-[winfo reqwidth $w])/2}]\n\
set y [expr {([winfo screenheight $w]-[winfo reqheight $w])/2}]\n\
set checkBounds 0\n\
} elseif {[string equal -length [string length $place] $place \"pointer\"]} {\n\
if {[string equal -length [string length $anchor] $anchor \"center\"]} {\n\
set x [expr {[winfo pointerx $w]-[winfo reqwidth $w]/2}]\n\
set y [expr {[winfo pointery $w]-[winfo reqheight $w]/2}]\n\
} else {\n\
set x [winfo pointerx $w]\n\
set y [winfo pointery $w]\n\
}\n\
} elseif {[string equal -length [string length $place] $place \"widget\"] && \\\n\
[winfo exists $anchor] && [winfo ismapped $anchor]} {\n\
set x [expr {[winfo rootx $anchor] + \\\n\
([winfo width $anchor]-[winfo reqwidth $w])/2}]\n\
set y [expr {[winfo rooty $anchor] + \\\n\
([winfo height $anchor]-[winfo reqheight $w])/2}]\n\
} else {\n\
set x [expr {([winfo screenwidth $w]-[winfo reqwidth $w])/2}]\n\
set y [expr {([winfo screenheight $w]-[winfo reqheight $w])/2}]\n\
set checkBounds 0\n\
}\n\
if {[tk windowingsystem] eq \"win32\"} {\n\
set checkBounds 0\n\
}\n\
if {$checkBounds} {\n\
if {$x < 0} {\n\
set x 0\n\
} elseif {$x > ([winfo screenwidth $w]-[winfo reqwidth $w])} {\n\
set x [expr {[winfo screenwidth $w]-[winfo reqwidth $w]}]\n\
}\n\
if {$y < 0} {\n\
set y 0\n\
} elseif {$y > ([winfo screenheight $w]-[winfo reqheight $w])} {\n\
set y [expr {[winfo screenheight $w]-[winfo reqheight $w]}]\n\
}\n\
if {[tk windowingsystem] eq \"aqua\"} {\n\
if {$y < 22} { set y 22 }\n\
}\n\
}\n\
wm geometry $w +$x+$y\n\
wm deiconify $w\n\
}\n\
\n\
proc ::tk::SetFocusGrab {grab {focus {}}} {\n\
set index \"$grab,$focus\"\n\
upvar ::tk::FocusGrab($index) data\n\
\n\
lappend data [focus]\n\
set oldGrab [grab current $grab]\n\
lappend data $oldGrab\n\
if {[winfo exists $oldGrab]} {\n\
lappend data [grab status $oldGrab]\n\
}\n\
catch {grab $grab}\n\
if {[winfo exists $focus]} {\n\
focus $focus\n\
}\n\
}\n\
\n\
proc ::tk::RestoreFocusGrab {grab focus {destroy destroy}} {\n\
set index \"$grab,$focus\"\n\
if {[info exists ::tk::FocusGrab($index)]} {\n\
foreach {oldFocus oldGrab oldStatus} $::tk::FocusGrab($index) { break }\n\
unset ::tk::FocusGrab($index)\n\
} else {\n\
set oldGrab \"\"\n\
}\n\
\n\
catch {focus $oldFocus}\n\
grab release $grab\n\
if {$destroy eq \"withdraw\"} {\n\
wm withdraw $grab\n\
} else {\n\
destroy $grab\n\
}\n\
if {[winfo exists $oldGrab] && [winfo ismapped $oldGrab]} {\n\
if {$oldStatus eq \"global\"} {\n\
grab -global $oldGrab\n\
} else {\n\
grab $oldGrab\n\
}\n\
}\n\
}\n\
\n\
if {$tcl_platform(platform) eq \"unix\"} {\n\
proc ::tk::GetSelection {w {sel PRIMARY}} {\n\
if {[catch {selection get -displayof $w -selection $sel \\\n\
-type UTF8_STRING} txt] \\\n\
&& [catch {selection get -displayof $w -selection $sel} txt]} {\n\
return -code error \"could not find default selection\"\n\
} else {\n\
return $txt\n\
}\n\
}\n\
} else {\n\
proc ::tk::GetSelection {w {sel PRIMARY}} {\n\
if {[catch {selection get -displayof $w -selection $sel} txt]} {\n\
return -code error \"could not find default selection\"\n\
} else {\n\
return $txt\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScreenChanged {screen} {\n\
set x [string last . $screen]\n\
if {$x > 0} {\n\
set disp [string range $screen 0 [expr {$x - 1}]]\n\
} else {\n\
set disp $screen\n\
}\n\
\n\
set disp [string map {:: _doublecolon_} $disp]\n\
\n\
uplevel #0 [list upvar #0 ::tk::Priv.$disp ::tk::Priv]\n\
variable ::tk::Priv\n\
global tcl_platform\n\
\n\
if {[info exists Priv]} {\n\
set Priv(screen) $screen\n\
return\n\
}\n\
array set Priv {\n\
activeMenu	{}\n\
activeItem	{}\n\
afterId		{}\n\
buttons		0\n\
buttonWindow	{}\n\
dragging	0\n\
focus		{}\n\
grab		{}\n\
initPos		{}\n\
inMenubutton	{}\n\
listboxPrev	{}\n\
menuBar		{}\n\
mouseMoved	0\n\
oldGrab		{}\n\
popup		{}\n\
postedMb	{}\n\
pressX		0\n\
pressY		0\n\
prevPos		0\n\
selectMode	char\n\
}\n\
set Priv(screen) $screen\n\
set Priv(tearoff) [string equal [tk windowingsystem] \"x11\"]\n\
set Priv(window) {}\n\
}\n\
\n\
\n\
tk::ScreenChanged [winfo screen .]\n\
\n\
\n\
proc ::tk::EventMotifBindings {n1 dummy dummy} {\n\
upvar $n1 name\n\
\n\
if {$name} {\n\
set op delete\n\
} else {\n\
set op add\n\
}\n\
\n\
event $op <<Cut>> <Control-Key-w>\n\
event $op <<Copy>> <Meta-Key-w> \n\
event $op <<Paste>> <Control-Key-y>\n\
event $op <<Undo>> <Control-underscore>\n\
}\n\
\n\
\n\
if {![llength [info commands tk_chooseColor]]} {\n\
proc ::tk_chooseColor {args} {\n\
return [tk::dialog::color:: {*}$args]\n\
}\n\
}\n\
if {![llength [info commands tk_getOpenFile]]} {\n\
proc ::tk_getOpenFile {args} {\n\
if {$::tk_strictMotif} {\n\
return [tk::MotifFDialog open {*}$args]\n\
} else {\n\
return [::tk::dialog::file:: open {*}$args]\n\
}\n\
}\n\
}\n\
if {![llength [info commands tk_getSaveFile]]} {\n\
proc ::tk_getSaveFile {args} {\n\
if {$::tk_strictMotif} {\n\
return [tk::MotifFDialog save {*}$args]\n\
} else {\n\
return [::tk::dialog::file:: save {*}$args]\n\
}\n\
}\n\
}\n\
if {![llength [info commands tk_messageBox]]} {\n\
proc ::tk_messageBox {args} {\n\
return [tk::MessageBox {*}$args]\n\
}\n\
}\n\
if {![llength [info command tk_chooseDirectory]]} {\n\
proc ::tk_chooseDirectory {args} {\n\
return [::tk::dialog::file::chooseDir:: {*}$args]\n\
}\n\
}\n\
\n\
\n\
switch -exact -- [tk windowingsystem] {\n\
\"x11\" {\n\
event add <<Cut>> <Control-Key-x> <Key-F20> <Control-Lock-Key-X>\n\
event add <<Copy>> <Control-Key-c> <Key-F16> <Control-Lock-Key-C>\n\
event add <<Paste>> <Control-Key-v> <Key-F18> <Control-Lock-Key-V>\n\
event add <<PasteSelection>> <ButtonRelease-2>\n\
event add <<Undo>> <Control-Key-z> <Control-Lock-Key-Z>\n\
event add <<Redo>> <Control-Key-Z> <Control-Lock-Key-z>\n\
catch { event add <<PrevWindow>> <ISO_Left_Tab> }\n\
catch { event add <<PrevWindow>> <hpBackTab> }\n\
\n\
trace add variable ::tk_strictMotif write ::tk::EventMotifBindings\n\
set ::tk_strictMotif $::tk_strictMotif\n\
set ::tk::AlwaysShowSelection 1\n\
}\n\
\"win32\" {\n\
event add <<Cut>> <Control-Key-x> <Shift-Key-Delete> \\\n\
<Control-Lock-Key-X>\n\
event add <<Copy>> <Control-Key-c> <Control-Key-Insert> \\\n\
<Control-Lock-Key-C>\n\
event add <<Paste>> <Control-Key-v> <Shift-Key-Insert> \\\n\
<Control-Lock-Key-V>\n\
event add <<PasteSelection>> <ButtonRelease-2>\n\
event add <<Undo>> <Control-Key-z> <Control-Lock-Key-Z>\n\
event add <<Redo>> <Control-Key-y> <Control-Lock-Key-Y>\n\
}\n\
\"aqua\" {\n\
event add <<Cut>> <Command-Key-x> <Key-F2> <Control-Lock-Key-X>\n\
event add <<Copy>> <Command-Key-c> <Key-F3> <Control-Lock-Key-C>\n\
event add <<Paste>> <Command-Key-v> <Key-F4> <Control-Lock-Key-V>\n\
event add <<PasteSelection>> <ButtonRelease-2>\n\
event add <<Clear>> <Clear>\n\
event add <<Undo>> <Command-Key-z> <Control-Lock-Key-Z>\n\
event add <<Redo>> <Command-Key-y> <Control-Lock-Key-Y>\n\
}\n\
}\n\
\n\
\n\
if {$::tk_library ne \"\"} {\n\
proc ::tk::SourceLibFile {file} {\n\
namespace eval :: [list source [file join $::tk_library $file.tcl]]\n\
}\n\
namespace eval ::tk {\n\
SourceLibFile button\n\
SourceLibFile entry\n\
SourceLibFile listbox\n\
SourceLibFile menu\n\
SourceLibFile panedwindow\n\
SourceLibFile scale\n\
SourceLibFile scrlbar\n\
SourceLibFile spinbox\n\
SourceLibFile text\n\
}\n\
}\n\
\n\
\n\
event add <<PrevWindow>> <Shift-Tab>\n\
bind all <Tab> {tk::TabToWindow [tk_focusNext %W]}\n\
bind all <<PrevWindow>> {tk::TabToWindow [tk_focusPrev %W]}\n\
\n\
\n\
proc ::tk::CancelRepeat {} {\n\
variable ::tk::Priv\n\
after cancel $Priv(afterId)\n\
set Priv(afterId) {}\n\
}\n\
\n\
\n\
proc ::tk::TabToWindow {w} {\n\
set focus [focus]\n\
if {$focus ne \"\"} {\n\
event generate $focus <<TraverseOut>>\n\
}\n\
focus $w\n\
event generate $w <<TraverseIn>>\n\
}\n\
\n\
proc ::tk::UnderlineAmpersand {text} {\n\
set s [string map {&& & & \\ufeff} $text]\n\
set idx [string first \\ufeff $s]\n\
return [list [string map {\\ufeff {}} $s] $idx]\n\
}\n\
\n\
proc ::tk::SetAmpText {widget text} {\n\
lassign [UnderlineAmpersand $text] newtext under\n\
$widget configure -text $newtext -underline $under\n\
}\n\
\n\
proc ::tk::AmpWidget {class path args} {\n\
set options {}\n\
foreach {opt val} $args {\n\
if {$opt eq \"-text\"} {\n\
lassign [UnderlineAmpersand $val] newtext under\n\
lappend options -text $newtext -underline $under\n\
} else {\n\
lappend options $opt $val\n\
}\n\
}\n\
set result [$class $path {*}$options]\n\
if {[string match \"*button\" $class]} {\n\
bind $path <<AltUnderlined>> [list $path invoke]\n\
}\n\
return $result\n\
}\n\
\n\
proc ::tk::AmpMenuArgs {widget add type args} {\n\
set options {}\n\
foreach {opt val} $args {\n\
if {$opt eq \"-label\"} {\n\
lassign [UnderlineAmpersand $val] newlabel under\n\
lappend options -label $newlabel -underline $under\n\
} else {\n\
lappend options $opt $val\n\
}\n\
}\n\
$widget add $type {*}$options\n\
}\n\
\n\
proc ::tk::FindAltKeyTarget {path char} {\n\
switch -- [winfo class $path] {\n\
Button - Label - \n\
TButton - TLabel - TCheckbutton {\n\
if {[string equal -nocase $char \\\n\
[string index [$path cget -text] [$path cget -underline]]]} {\n\
return $path\n\
} else {\n\
return {}\n\
}\n\
}\n\
default {\n\
foreach child [concat [grid slaves $path] \\\n\
[pack slaves $path] [place slaves $path]] {\n\
set target [FindAltKeyTarget $child $char]\n\
if {$target ne \"\"} {\n\
return $target\n\
}\n\
}\n\
}\n\
}\n\
return {}\n\
}\n\
\n\
proc ::tk::AltKeyInDialog {path key} {\n\
set target [FindAltKeyTarget $path $key]\n\
if { $target eq \"\"} return\n\
event generate $target <<AltUnderlined>>\n\
}\n\
\n\
\n\
proc ::tk::mcmaxamp {args} {\n\
set maxlen 0\n\
foreach arg $args {\n\
lassign [UnderlineAmpersand [mc $arg]] msg\n\
set length [string length $msg]\n\
if {$length > $maxlen} {\n\
set maxlen $length\n\
}\n\
}\n\
return $maxlen\n\
}\n\
\n\
\n\
if {[tk windowingsystem] eq \"aqua\"} {\n\
namespace eval ::tk::mac {\n\
variable useCustomMDEF 0\n\
}\n\
}\n\
\n\
if {$::ttk::library ne \"\"} {\n\
uplevel \\#0 [list source [file join $::ttk::library ttk.tcl]]\n\
}\n\
\n\
\n\
namespace eval ::tk {}\n\
namespace eval ::tk::dialog {}\n\
namespace eval ::tk::dialog::color {\n\
namespace import ::tk::msgcat::*\n\
}\n\
\n\
proc ::tk::dialog::color:: {args} {\n\
variable ::tk::Priv\n\
set dataName __tk__color\n\
upvar ::tk::dialog::color::$dataName data\n\
set w .$dataName\n\
\n\
set data(lines,red,start)   0\n\
set data(lines,red,last)   -1\n\
set data(lines,green,start) 0\n\
set data(lines,green,last) -1\n\
set data(lines,blue,start)  0\n\
set data(lines,blue,last)  -1\n\
\n\
set data(NUM_COLORBARS) 16\n\
\n\
set data(BARS_WIDTH) 160\n\
\n\
set data(PLGN_HEIGHT) 10\n\
\n\
set data(PLGN_WIDTH) 10\n\
\n\
Config $dataName $args\n\
InitValues $dataName\n\
\n\
set sc [winfo screen $data(-parent)]\n\
set winExists [winfo exists $w]\n\
if {!$winExists || $sc ne [winfo screen $w]} {\n\
if {$winExists} {\n\
destroy $w\n\
}\n\
toplevel $w -class TkColorDialog -screen $sc\n\
if {[tk windowingsystem] eq \"x11\"} {wm attributes $w -type dialog}\n\
BuildDialog $w\n\
}\n\
\n\
\n\
if {[winfo viewable [winfo toplevel $data(-parent)]] } {\n\
wm transient $w $data(-parent)\n\
}\n\
\n\
\n\
::tk::PlaceWindow $w widget $data(-parent)\n\
wm title $w $data(-title)\n\
\n\
\n\
::tk::SetFocusGrab $w $data(okBtn)\n\
\n\
\n\
vwait ::tk::Priv(selectColor)\n\
set result $Priv(selectColor)\n\
::tk::RestoreFocusGrab $w $data(okBtn)\n\
unset data\n\
\n\
return $result\n\
}\n\
\n\
proc ::tk::dialog::color::InitValues {dataName} {\n\
upvar ::tk::dialog::color::$dataName data\n\
\n\
set data(intensityIncr) [expr {256 / $data(NUM_COLORBARS)}]\n\
\n\
set data(colorbarWidth) [expr {$data(BARS_WIDTH) / $data(NUM_COLORBARS)}]\n\
\n\
set data(indent) [expr {$data(PLGN_WIDTH) / 2}]\n\
\n\
set data(colorPad) 2\n\
set data(selPad)   [expr {$data(PLGN_WIDTH) / 2}]\n\
\n\
set data(minX) $data(indent)\n\
\n\
set data(maxX) [expr {$data(BARS_WIDTH) + $data(indent)-1}]\n\
\n\
set data(canvasWidth) [expr {$data(BARS_WIDTH) + $data(PLGN_WIDTH)}]\n\
\n\
set data(selection) $data(-initialcolor)\n\
set data(finalColor)  $data(-initialcolor)\n\
set rgb [winfo rgb . $data(selection)]\n\
\n\
set data(red,intensity)   [expr {[lindex $rgb 0]/0x100}]\n\
set data(green,intensity) [expr {[lindex $rgb 1]/0x100}]\n\
set data(blue,intensity)  [expr {[lindex $rgb 2]/0x100}]\n\
}\n\
\n\
proc ::tk::dialog::color::Config {dataName argList} {\n\
variable ::tk::Priv\n\
upvar ::tk::dialog::color::$dataName data\n\
\n\
if {[info exists Priv(selectColor)] && $Priv(selectColor) ne \"\"} {\n\
set defaultColor $Priv(selectColor)\n\
} else {\n\
set defaultColor [. cget -background]\n\
}\n\
\n\
set specs [list \\\n\
[list -initialcolor \"\" \"\" $defaultColor] \\\n\
[list -parent \"\" \"\" \".\"] \\\n\
[list -title \"\" \"\" [mc \"Color\"]] \\\n\
]\n\
\n\
tclParseConfigSpec ::tk::dialog::color::$dataName $specs \"\" $argList\n\
\n\
if {$data(-title) eq \"\"} {\n\
set data(-title) \" \"\n\
}\n\
if {[catch {winfo rgb . $data(-initialcolor)} err]} {\n\
error $err\n\
}\n\
\n\
if {![winfo exists $data(-parent)]} {\n\
error \"bad window path name \\\"$data(-parent)\\\"\"\n\
}\n\
}\n\
\n\
proc ::tk::dialog::color::BuildDialog {w} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
set topFrame [frame $w.top -relief raised -bd 1]\n\
\n\
set stripsFrame [frame $topFrame.colorStrip]\n\
\n\
set maxWidth [::tk::mcmaxamp &Red &Green &Blue]\n\
set maxWidth [expr {$maxWidth<6 ? 6 : $maxWidth}]\n\
set colorList {\n\
red   \"&Red\"\n\
green \"&Green\"\n\
blue  \"&Blue\"\n\
}\n\
foreach {color l} $colorList {\n\
set f [frame $stripsFrame.$color]\n\
\n\
set box [frame $f.box]\n\
\n\
::tk::AmpWidget label $box.label -text \"[mc $l]:\" \\\n\
-width $maxWidth -anchor ne\n\
bind $box.label <<AltUnderlined>> [list focus $box.entry]\n\
\n\
entry $box.entry -textvariable \\\n\
::tk::dialog::color::[winfo name $w]($color,intensity) \\\n\
-width 4\n\
pack $box.label -side left -fill y -padx 2 -pady 3\n\
pack $box.entry -side left -anchor n -pady 0\n\
pack $box -side left -fill both\n\
\n\
set height [expr {\n\
[winfo reqheight $box.entry] -\n\
2*([$box.entry cget -highlightthickness] + [$box.entry cget -bd])\n\
}]\n\
\n\
canvas $f.color -height $height \\\n\
-width $data(BARS_WIDTH) -relief sunken -bd 2\n\
canvas $f.sel -height $data(PLGN_HEIGHT) \\\n\
-width $data(canvasWidth) -highlightthickness 0\n\
pack $f.color -expand yes -fill both\n\
pack $f.sel -expand yes -fill both\n\
\n\
pack $f -side top -fill x -padx 0 -pady 2\n\
\n\
set data($color,entry) $box.entry\n\
set data($color,col) $f.color\n\
set data($color,sel) $f.sel\n\
\n\
bind $data($color,col) <Configure> \\\n\
[list tk::dialog::color::DrawColorScale $w $color 1]\n\
bind $data($color,col) <Enter> \\\n\
[list tk::dialog::color::EnterColorBar $w $color]\n\
bind $data($color,col) <Leave> \\\n\
[list tk::dialog::color::LeaveColorBar $w $color]\n\
\n\
bind $data($color,sel) <Enter> \\\n\
[list tk::dialog::color::EnterColorBar $w $color]\n\
bind $data($color,sel) <Leave> \\\n\
[list tk::dialog::color::LeaveColorBar $w $color]\n\
\n\
bind $box.entry <Return> [list tk::dialog::color::HandleRGBEntry $w]\n\
}\n\
\n\
pack $stripsFrame -side left -fill both -padx 4 -pady 10\n\
\n\
set selFrame [frame $topFrame.sel]\n\
set lab [::tk::AmpWidget label $selFrame.lab \\\n\
-text [mc \"&Selection:\"] -anchor sw]\n\
set ent [entry $selFrame.ent \\\n\
-textvariable ::tk::dialog::color::[winfo name $w](selection) \\\n\
-width 16]\n\
set f1  [frame $selFrame.f1 -relief sunken -bd 2]\n\
set data(finalCanvas) [frame $f1.demo -bd 0 -width 100 -height 70]\n\
\n\
pack $lab $ent -side top -fill x -padx 4 -pady 2\n\
pack $f1 -expand yes -anchor nw -fill both -padx 6 -pady 10\n\
pack $data(finalCanvas) -expand yes -fill both\n\
\n\
bind $ent <Return> [list tk::dialog::color::HandleSelEntry $w]\n\
\n\
pack $selFrame -side left -fill none -anchor nw\n\
pack $topFrame -side top -expand yes -fill both -anchor nw\n\
\n\
set botFrame [frame $w.bot -relief raised -bd 1]\n\
\n\
::tk::AmpWidget button $botFrame.ok     -text [mc \"&OK\"]		\\\n\
-command [list tk::dialog::color::OkCmd $w]\n\
::tk::AmpWidget button $botFrame.cancel -text [mc \"&Cancel\"]	\\\n\
-command [list tk::dialog::color::CancelCmd $w]\n\
\n\
set data(okBtn)      $botFrame.ok\n\
set data(cancelBtn)  $botFrame.cancel\n\
\n\
grid x $botFrame.ok x $botFrame.cancel x -sticky ew\n\
grid configure $botFrame.ok $botFrame.cancel -padx 10 -pady 10\n\
grid columnconfigure $botFrame {0 4} -weight 1 -uniform space\n\
grid columnconfigure $botFrame {1 3} -weight 1 -uniform button\n\
grid columnconfigure $botFrame 2 -weight 2 -uniform space\n\
pack $botFrame -side bottom -fill x\n\
\n\
bind $lab <<AltUnderlined>> [list focus $ent]\n\
bind $w <KeyPress-Escape> [list tk::ButtonInvoke $data(cancelBtn)]\n\
bind $w <Alt-Key> [list tk::AltKeyInDialog $w %A]\n\
\n\
wm protocol $w WM_DELETE_WINDOW [list tk::dialog::color::CancelCmd $w]\n\
bind $lab <Destroy> [list tk::dialog::color::CancelCmd $w]\n\
}\n\
\n\
proc ::tk::dialog::color::SetRGBValue {w color} {\n\
upvar ::tk::dialog::color::[winfo name $w] data \n\
\n\
set data(red,intensity)   [lindex $color 0]\n\
set data(green,intensity) [lindex $color 1]\n\
set data(blue,intensity)  [lindex $color 2]\n\
\n\
RedrawColorBars $w all\n\
\n\
foreach color {red green blue} {\n\
set x [RgbToX $w $data($color,intensity)]\n\
MoveSelector $w $data($color,sel) $color $x 0\n\
}\n\
}\n\
\n\
proc ::tk::dialog::color::XToRgb {w x} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
set x [expr {($x * $data(intensityIncr))/ $data(colorbarWidth)}]\n\
if {$x > 255} {\n\
set x 255\n\
}\n\
return $x\n\
}\n\
\n\
proc ::tk::dialog::color::RgbToX {w color} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
return [expr {($color * $data(colorbarWidth)/ $data(intensityIncr))}]\n\
}\n\
\n\
proc ::tk::dialog::color::DrawColorScale {w c {create 0}} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
set col $data($c,col)\n\
set sel $data($c,sel)\n\
\n\
if {$create} {\n\
if { $data(lines,$c,last) > $data(lines,$c,start)} {\n\
for {set i $data(lines,$c,start)} \\\n\
{$i <= $data(lines,$c,last)} {incr i} {\n\
$sel delete $i\n\
}\n\
}\n\
if {[info exists data($c,index)]} {\n\
$sel delete $data($c,index)\n\
}\n\
\n\
CreateSelector $w $sel $c\n\
$sel bind $data($c,index) <ButtonPress-1> \\\n\
[list tk::dialog::color::StartMove $w $sel $c %x $data(selPad) 1]\n\
$sel bind $data($c,index) <B1-Motion> \\\n\
[list tk::dialog::color::MoveSelector $w $sel $c %x $data(selPad)]\n\
$sel bind $data($c,index) <ButtonRelease-1> \\\n\
[list tk::dialog::color::ReleaseMouse $w $sel $c %x $data(selPad)]\n\
\n\
set height [winfo height $col]\n\
set data($c,clickRegion) [$sel create rectangle 0 0 \\\n\
$data(canvasWidth) $height -fill {} -outline {}]\n\
\n\
bind $col <ButtonPress-1> \\\n\
[list tk::dialog::color::StartMove $w $sel $c %x $data(colorPad)]\n\
bind $col <B1-Motion> \\\n\
[list tk::dialog::color::MoveSelector $w $sel $c %x $data(colorPad)]\n\
bind $col <ButtonRelease-1> \\\n\
[list tk::dialog::color::ReleaseMouse $w $sel $c %x $data(colorPad)]\n\
\n\
$sel bind $data($c,clickRegion) <ButtonPress-1> \\\n\
[list tk::dialog::color::StartMove $w $sel $c %x $data(selPad)]\n\
$sel bind $data($c,clickRegion) <B1-Motion> \\\n\
[list tk::dialog::color::MoveSelector $w $sel $c %x $data(selPad)]\n\
$sel bind $data($c,clickRegion) <ButtonRelease-1> \\\n\
[list tk::dialog::color::ReleaseMouse $w $sel $c %x $data(selPad)]\n\
} else {\n\
set l $data(lines,$c,start)\n\
}\n\
\n\
set highlightW [expr {[$col cget -highlightthickness] + [$col cget -bd]}]\n\
for {set i 0} { $i < $data(NUM_COLORBARS)} { incr i} {\n\
set intensity [expr {$i * $data(intensityIncr)}]\n\
set startx [expr {$i * $data(colorbarWidth) + $highlightW}]\n\
if {$c eq \"red\"} {\n\
set color [format \"#%02x%02x%02x\" \\\n\
$intensity $data(green,intensity) $data(blue,intensity)]\n\
} elseif {$c eq \"green\"} {\n\
set color [format \"#%02x%02x%02x\" \\\n\
$data(red,intensity) $intensity $data(blue,intensity)]\n\
} else {\n\
set color [format \"#%02x%02x%02x\" \\\n\
$data(red,intensity) $data(green,intensity) $intensity]\n\
}\n\
\n\
if {$create} {\n\
set index [$col create rect $startx $highlightW \\\n\
[expr {$startx +$data(colorbarWidth)}] \\\n\
[expr {[winfo height $col] + $highlightW}] \\\n\
-fill $color -outline $color]\n\
} else {\n\
$col itemconfigure $l -fill $color -outline $color\n\
incr l\n\
}\n\
}\n\
$sel raise $data($c,index)\n\
\n\
if {$create} {\n\
set data(lines,$c,last) $index\n\
set data(lines,$c,start) [expr {$index - $data(NUM_COLORBARS) + 1}]\n\
}\n\
\n\
RedrawFinalColor $w\n\
}\n\
\n\
proc ::tk::dialog::color::CreateSelector {w sel c } {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
set data($c,index) [$sel create polygon \\\n\
0 $data(PLGN_HEIGHT) \\\n\
$data(PLGN_WIDTH) $data(PLGN_HEIGHT) \\\n\
$data(indent) 0]\n\
set data($c,x) [RgbToX $w $data($c,intensity)]\n\
$sel move $data($c,index) $data($c,x) 0\n\
}\n\
\n\
proc ::tk::dialog::color::RedrawFinalColor {w} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
set color [format \"#%02x%02x%02x\" $data(red,intensity) \\\n\
$data(green,intensity) $data(blue,intensity)]\n\
\n\
$data(finalCanvas) configure -bg $color\n\
set data(finalColor) $color\n\
set data(selection) $color\n\
set data(finalRGB) [list \\\n\
$data(red,intensity) \\\n\
$data(green,intensity) \\\n\
$data(blue,intensity)]\n\
}\n\
\n\
proc ::tk::dialog::color::RedrawColorBars {w colorChanged} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
switch $colorChanged {\n\
red { \n\
DrawColorScale $w green\n\
DrawColorScale $w blue\n\
}\n\
green {\n\
DrawColorScale $w red\n\
DrawColorScale $w blue\n\
}\n\
blue {\n\
DrawColorScale $w red\n\
DrawColorScale $w green\n\
}\n\
default {\n\
DrawColorScale $w red\n\
DrawColorScale $w green\n\
DrawColorScale $w blue\n\
}\n\
}\n\
RedrawFinalColor $w\n\
}\n\
\n\
\n\
proc ::tk::dialog::color::StartMove {w sel color x delta {dontMove 0}} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
if {!$dontMove} {\n\
MoveSelector $w $sel $color $x $delta\n\
}\n\
}\n\
\n\
proc ::tk::dialog::color::MoveSelector {w sel color x delta} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
incr x -$delta\n\
\n\
if { $x < 0 } {\n\
set x 0\n\
} elseif { $x > $data(BARS_WIDTH)} {\n\
set x $data(BARS_WIDTH)\n\
}\n\
set diff [expr {$x - $data($color,x)}]\n\
$sel move $data($color,index) $diff 0\n\
set data($color,x) [expr {$data($color,x) + $diff}]\n\
\n\
return $x\n\
}\n\
\n\
proc ::tk::dialog::color::ReleaseMouse {w sel color x delta} {\n\
upvar ::tk::dialog::color::[winfo name $w] data \n\
\n\
set x [MoveSelector $w $sel $color $x $delta]\n\
\n\
set data($color,intensity) [XToRgb $w $x]\n\
\n\
RedrawColorBars $w $color\n\
}\n\
\n\
proc ::tk::dialog::color::ResizeColorBars {w} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
if {\n\
($data(BARS_WIDTH) < $data(NUM_COLORBARS)) || \n\
(($data(BARS_WIDTH) % $data(NUM_COLORBARS)) != 0)\n\
} then {\n\
set data(BARS_WIDTH) $data(NUM_COLORBARS)\n\
}\n\
InitValues [winfo name $w]\n\
foreach color {red green blue} {\n\
$data($color,col) configure -width $data(canvasWidth)\n\
DrawColorScale $w $color 1\n\
}\n\
}\n\
\n\
proc ::tk::dialog::color::HandleSelEntry {w} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
set text [string trim $data(selection)]\n\
if {[catch {set color [winfo rgb . $text]} ]} {\n\
set data(selection) $data(finalColor)\n\
return\n\
}\n\
\n\
set R [expr {[lindex $color 0]/0x100}]\n\
set G [expr {[lindex $color 1]/0x100}]\n\
set B [expr {[lindex $color 2]/0x100}]\n\
\n\
SetRGBValue $w \"$R $G $B\"\n\
set data(selection) $text\n\
}\n\
\n\
proc ::tk::dialog::color::HandleRGBEntry {w} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
foreach c {red green blue} {\n\
if {[catch {\n\
set data($c,intensity) [expr {int($data($c,intensity))}]\n\
}]} {\n\
set data($c,intensity) 0\n\
}\n\
\n\
if {$data($c,intensity) < 0} {\n\
set data($c,intensity) 0\n\
}\n\
if {$data($c,intensity) > 255} {\n\
set data($c,intensity) 255\n\
}\n\
}\n\
\n\
SetRGBValue $w \"$data(red,intensity) \\\n\
$data(green,intensity) $data(blue,intensity)\"\n\
}    \n\
\n\
proc ::tk::dialog::color::EnterColorBar {w color} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
$data($color,sel) itemconfigure $data($color,index) -fill red\n\
}\n\
\n\
proc ::tk::dialog::color::LeaveColorBar {w color} {\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
$data($color,sel) itemconfigure $data($color,index) -fill black\n\
}\n\
\n\
proc ::tk::dialog::color::OkCmd {w} {\n\
variable ::tk::Priv\n\
upvar ::tk::dialog::color::[winfo name $w] data\n\
\n\
set Priv(selectColor) $data(finalColor)\n\
}\n\
\n\
proc ::tk::dialog::color::CancelCmd {w} {\n\
variable ::tk::Priv\n\
set Priv(selectColor) \"\"\n\
}\n\
\n\
proc tclParseConfigSpec {w specs flags argList} {\n\
upvar #0 $w data\n\
\n\
foreach spec $specs {\n\
if {[llength $spec] < 4} {\n\
error \"\\\"spec\\\" should contain 5 or 4 elements\"\n\
}\n\
set cmdsw [lindex $spec 0]\n\
set cmd($cmdsw) \"\"\n\
set rname($cmdsw)   [lindex $spec 1]\n\
set rclass($cmdsw)  [lindex $spec 2]\n\
set def($cmdsw)     [lindex $spec 3]\n\
set verproc($cmdsw) [lindex $spec 4]\n\
}\n\
\n\
if {[llength $argList] & 1} {\n\
set cmdsw [lindex $argList end]\n\
if {![info exists cmd($cmdsw)]} {\n\
error \"bad option \\\"$cmdsw\\\": must be [tclListValidFlags cmd]\"\n\
}\n\
error \"value for \\\"$cmdsw\\\" missing\"\n\
}\n\
\n\
foreach cmdsw [array names cmd] {\n\
set data($cmdsw) $def($cmdsw)\n\
}\n\
\n\
foreach {cmdsw value} $argList {\n\
if {![info exists cmd($cmdsw)]} {\n\
error \"bad option \\\"$cmdsw\\\": must be [tclListValidFlags cmd]\"\n\
}\n\
set data($cmdsw) $value\n\
}\n\
\n\
}\n\
\n\
proc tclListValidFlags {v} {\n\
upvar $v cmd\n\
\n\
set len [llength [array names cmd]]\n\
set i 1\n\
set separator \"\"\n\
set errormsg \"\"\n\
foreach cmdsw [lsort [array names cmd]] {\n\
append errormsg \"$separator$cmdsw\"\n\
incr i\n\
if {$i == $len} {\n\
set separator \", or \"\n\
} else {\n\
set separator \", \"\n\
}\n\
}\n\
return $errormsg\n\
}\n\
\n\
\n\
\n\
proc ::tk::FocusGroup_Create {t} {\n\
variable ::tk::Priv\n\
if {[winfo toplevel $t] ne $t} {\n\
error \"$t is not a toplevel window\"\n\
}\n\
if {![info exists Priv(fg,$t)]} {\n\
set Priv(fg,$t) 1\n\
set Priv(focus,$t) \"\"\n\
bind $t <FocusIn>  [list tk::FocusGroup_In  $t %W %d]\n\
bind $t <FocusOut> [list tk::FocusGroup_Out $t %W %d]\n\
bind $t <Destroy>  [list tk::FocusGroup_Destroy $t %W]\n\
}\n\
}\n\
\n\
proc ::tk::FocusGroup_BindIn {t w cmd} {\n\
variable FocusIn\n\
variable ::tk::Priv\n\
if {![info exists Priv(fg,$t)]} {\n\
error \"focus group \\\"$t\\\" doesn't exist\"\n\
}\n\
set FocusIn($t,$w) $cmd\n\
}\n\
\n\
\n\
proc ::tk::FocusGroup_BindOut {t w cmd} {\n\
variable FocusOut\n\
variable ::tk::Priv\n\
if {![info exists Priv(fg,$t)]} {\n\
error \"focus group \\\"$t\\\" doesn't exist\"\n\
}\n\
set FocusOut($t,$w) $cmd\n\
}\n\
\n\
proc ::tk::FocusGroup_Destroy {t w} {\n\
variable FocusIn\n\
variable FocusOut\n\
variable ::tk::Priv\n\
\n\
if {$t eq $w} {\n\
unset Priv(fg,$t)\n\
unset Priv(focus,$t) \n\
\n\
foreach name [array names FocusIn $t,*] {\n\
unset FocusIn($name)\n\
}\n\
foreach name [array names FocusOut $t,*] {\n\
unset FocusOut($name)\n\
}\n\
} else {\n\
if {[info exists Priv(focus,$t)] && ($Priv(focus,$t) eq $w)} {\n\
set Priv(focus,$t) \"\"\n\
}\n\
unset -nocomplain FocusIn($t,$w) FocusOut($t,$w)\n\
}\n\
}\n\
\n\
proc ::tk::FocusGroup_In {t w detail} {\n\
variable FocusIn\n\
variable ::tk::Priv\n\
\n\
if {$detail ne \"NotifyNonlinear\" && $detail ne \"NotifyNonlinearVirtual\"} {\n\
return\n\
}\n\
if {![info exists FocusIn($t,$w)]} {\n\
set FocusIn($t,$w) \"\"\n\
return\n\
}\n\
if {![info exists Priv(focus,$t)]} {\n\
return\n\
}\n\
if {$Priv(focus,$t) eq $w} {\n\
return\n\
} else {\n\
set Priv(focus,$t) $w\n\
eval $FocusIn($t,$w)\n\
}\n\
}\n\
\n\
proc ::tk::FocusGroup_Out {t w detail} {\n\
variable FocusOut\n\
variable ::tk::Priv\n\
\n\
if {$detail ne \"NotifyNonlinear\" && $detail ne \"NotifyNonlinearVirtual\"} {\n\
return\n\
}\n\
if {![info exists Priv(focus,$t)]} {\n\
return\n\
}\n\
if {![info exists FocusOut($t,$w)]} {\n\
return\n\
} else {\n\
eval $FocusOut($t,$w)\n\
set Priv(focus,$t) \"\"\n\
}\n\
}\n\
\n\
proc ::tk::FDGetFileTypes {string} {\n\
foreach t $string {\n\
if {[llength $t] < 2 || [llength $t] > 3} {\n\
error \"bad file type \\\"$t\\\", should be \\\"typeName {extension ?extensions ...?} ?{macType ?macTypes ...?}?\\\"\"\n\
}\n\
lappend fileTypes([lindex $t 0]) {*}[lindex $t 1]\n\
}\n\
\n\
set types {}\n\
foreach t $string {\n\
set label [lindex $t 0]\n\
set exts {}\n\
\n\
if {[info exists hasDoneType($label)]} {\n\
continue\n\
}\n\
\n\
foreach macType [lindex $t 2] {\n\
if {[string length $macType] != 4} {\n\
error \"bad Macintosh file type \\\"$macType\\\"\"\n\
}\n\
}\n\
\n\
set name \"$label \\(\"\n\
set sep \"\"\n\
set doAppend 1\n\
foreach ext $fileTypes($label) {\n\
if {$ext eq \"\"} {\n\
continue\n\
}\n\
regsub {^[.]} $ext \"*.\" ext\n\
if {![info exists hasGotExt($label,$ext)]} {\n\
if {$doAppend} {\n\
if {[string length $sep] && [string length $name]>40} {\n\
set doAppend 0\n\
append name $sep...\n\
} else {\n\
append name $sep$ext\n\
}\n\
}\n\
lappend exts $ext\n\
set hasGotExt($label,$ext) 1\n\
}\n\
set sep \",\"\n\
}\n\
append name \"\\)\"\n\
lappend types [list $name $exts]\n\
\n\
set hasDoneType($label) 1\n\
}\n\
\n\
return $types\n\
}\n\
\n\
\n\
if {[tk windowingsystem] eq \"aqua\"} {\n\
bind Radiobutton <Enter> {\n\
tk::ButtonEnter %W\n\
}\n\
bind Radiobutton <1> {\n\
tk::ButtonDown %W\n\
}\n\
bind Radiobutton <ButtonRelease-1> {\n\
tk::ButtonUp %W\n\
}\n\
bind Checkbutton <Enter> {\n\
tk::ButtonEnter %W\n\
}\n\
bind Checkbutton <1> {\n\
tk::ButtonDown %W\n\
}\n\
bind Checkbutton <ButtonRelease-1> {\n\
tk::ButtonUp %W\n\
}\n\
bind Checkbutton <Leave> {\n\
tk::ButtonLeave %W\n\
}\n\
}\n\
if {\"windows\" eq $tcl_platform(platform)} {\n\
bind Checkbutton <equal> {\n\
tk::CheckRadioInvoke %W select\n\
}\n\
bind Checkbutton <plus> {\n\
tk::CheckRadioInvoke %W select\n\
}\n\
bind Checkbutton <minus> {\n\
tk::CheckRadioInvoke %W deselect\n\
}\n\
bind Checkbutton <1> {\n\
tk::CheckRadioDown %W\n\
}\n\
bind Checkbutton <ButtonRelease-1> {\n\
tk::ButtonUp %W\n\
}\n\
bind Checkbutton <Enter> {\n\
tk::CheckRadioEnter %W\n\
}\n\
bind Checkbutton <Leave> {\n\
tk::ButtonLeave %W\n\
}\n\
\n\
bind Radiobutton <1> {\n\
tk::CheckRadioDown %W\n\
}\n\
bind Radiobutton <ButtonRelease-1> {\n\
tk::ButtonUp %W\n\
}\n\
bind Radiobutton <Enter> {\n\
tk::CheckRadioEnter %W\n\
}\n\
}\n\
if {\"x11\" eq [tk windowingsystem]} {\n\
bind Checkbutton <Return> {\n\
if {!$tk_strictMotif} {\n\
tk::CheckInvoke %W\n\
}\n\
}\n\
bind Radiobutton <Return> {\n\
if {!$tk_strictMotif} {\n\
tk::CheckRadioInvoke %W\n\
}\n\
}\n\
bind Checkbutton <1> {\n\
tk::CheckInvoke %W\n\
}\n\
bind Radiobutton <1> {\n\
tk::CheckRadioInvoke %W\n\
}\n\
bind Checkbutton <Enter> {\n\
tk::CheckEnter %W\n\
}\n\
bind Radiobutton <Enter> {\n\
tk::ButtonEnter %W\n\
}\n\
bind Checkbutton <Leave> {\n\
tk::CheckLeave %W\n\
}\n\
}\n\
\n\
bind Button <space> {\n\
tk::ButtonInvoke %W\n\
}\n\
bind Checkbutton <space> {\n\
tk::CheckRadioInvoke %W\n\
}\n\
bind Radiobutton <space> {\n\
tk::CheckRadioInvoke %W\n\
}\n\
\n\
bind Button <FocusIn> {}\n\
bind Button <Enter> {\n\
tk::ButtonEnter %W\n\
}\n\
bind Button <Leave> {\n\
tk::ButtonLeave %W\n\
}\n\
bind Button <1> {\n\
tk::ButtonDown %W\n\
}\n\
bind Button <ButtonRelease-1> {\n\
tk::ButtonUp %W\n\
}\n\
\n\
bind Checkbutton <FocusIn> {}\n\
\n\
bind Radiobutton <FocusIn> {}\n\
bind Radiobutton <Leave> {\n\
tk::ButtonLeave %W\n\
}\n\
\n\
if {\"windows\" eq $tcl_platform(platform)} {\n\
\n\
\n\
\n\
proc ::tk::ButtonEnter w {\n\
variable ::tk::Priv\n\
if {[$w cget -state] ne \"disabled\"} {\n\
\n\
\n\
set Priv($w,relief) [$w cget -relief]\n\
if {$Priv(buttonWindow) eq $w} {\n\
$w configure -relief sunken -state active\n\
set Priv($w,prelief) sunken\n\
} elseif {[set over [$w cget -overrelief]] ne \"\"} {\n\
$w configure -relief $over\n\
set Priv($w,prelief) $over\n\
}\n\
}\n\
set Priv(window) $w\n\
}\n\
\n\
\n\
proc ::tk::ButtonLeave w {\n\
variable ::tk::Priv\n\
if {[$w cget -state] ne \"disabled\"} {\n\
$w configure -state normal\n\
}\n\
\n\
\n\
if {[info exists Priv($w,relief)]} {\n\
if {[info exists Priv($w,prelief)] && \\\n\
$Priv($w,prelief) eq [$w cget -relief]} {\n\
$w configure -relief $Priv($w,relief)\n\
}\n\
unset -nocomplain Priv($w,relief) Priv($w,prelief)\n\
}\n\
\n\
set Priv(window) \"\"\n\
}\n\
\n\
\n\
proc ::tk::ButtonDown w {\n\
variable ::tk::Priv\n\
\n\
\n\
if {![info exists Priv($w,relief)]} {\n\
set Priv($w,relief) [$w cget -relief]\n\
}\n\
\n\
if {[$w cget -state] ne \"disabled\"} {\n\
set Priv(buttonWindow) $w\n\
$w configure -relief sunken -state active\n\
set Priv($w,prelief) sunken\n\
\n\
after cancel $Priv(afterId)\n\
set delay [$w cget -repeatdelay]\n\
set Priv(repeated) 0\n\
if {$delay > 0} {\n\
set Priv(afterId) [after $delay [list tk::ButtonAutoInvoke $w]]\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ButtonUp w {\n\
variable ::tk::Priv\n\
if {$Priv(buttonWindow) eq $w} {\n\
set Priv(buttonWindow) \"\"\n\
\n\
\n\
if {[info exists Priv($w,relief)]} {\n\
if {[info exists Priv($w,prelief)] && \\\n\
$Priv($w,prelief) eq [$w cget -relief]} {\n\
$w configure -relief $Priv($w,relief)\n\
}\n\
unset -nocomplain Priv($w,relief) Priv($w,prelief)\n\
}\n\
\n\
after cancel $Priv(afterId)\n\
\n\
if {$Priv(window) eq $w && [$w cget -state] ne \"disabled\"} {\n\
$w configure -state normal\n\
\n\
if { $Priv(repeated) == 0 } {\n\
uplevel #0 [list $w invoke]\n\
}\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::CheckRadioEnter w {\n\
variable ::tk::Priv\n\
if {[$w cget -state] ne \"disabled\"} {\n\
if {$Priv(buttonWindow) eq $w} {\n\
$w configure -state active\n\
}\n\
if {[set over [$w cget -overrelief]] ne \"\"} {\n\
set Priv($w,relief)  [$w cget -relief]\n\
set Priv($w,prelief) $over\n\
$w configure -relief $over\n\
}\n\
}\n\
set Priv(window) $w\n\
}\n\
\n\
\n\
proc ::tk::CheckRadioDown w {\n\
variable ::tk::Priv\n\
if {![info exists Priv($w,relief)]} {\n\
set Priv($w,relief) [$w cget -relief]\n\
}\n\
if {[$w cget -state] ne \"disabled\"} {\n\
set Priv(buttonWindow) $w\n\
set Priv(repeated) 0\n\
$w configure -state active\n\
}\n\
}\n\
\n\
}\n\
\n\
if {\"x11\" eq [tk windowingsystem]} {\n\
\n\
\n\
\n\
proc ::tk::ButtonEnter {w} {\n\
variable ::tk::Priv\n\
if {[$w cget -state] ne \"disabled\"} {\n\
$w configure -state active\n\
\n\
\n\
set Priv($w,relief) [$w cget -relief]\n\
if {$Priv(buttonWindow) eq $w} {\n\
$w configure -relief sunken\n\
set Priv($w,prelief) sunken\n\
} elseif {[set over [$w cget -overrelief]] ne \"\"} {\n\
$w configure -relief $over\n\
set Priv($w,prelief) $over\n\
}\n\
}\n\
set Priv(window) $w\n\
}\n\
\n\
\n\
proc ::tk::ButtonLeave w {\n\
variable ::tk::Priv\n\
if {[$w cget -state] ne \"disabled\"} {\n\
$w configure -state normal\n\
}\n\
\n\
\n\
if {[info exists Priv($w,relief)]} {\n\
if {[info exists Priv($w,prelief)] && \\\n\
$Priv($w,prelief) eq [$w cget -relief]} {\n\
$w configure -relief $Priv($w,relief)\n\
}\n\
unset -nocomplain Priv($w,relief) Priv($w,prelief)\n\
}\n\
\n\
set Priv(window) \"\"\n\
}\n\
\n\
\n\
proc ::tk::ButtonDown w {\n\
variable ::tk::Priv\n\
\n\
\n\
if {![info exists Priv($w,relief)]} {\n\
set Priv($w,relief) [$w cget -relief]\n\
}\n\
\n\
if {[$w cget -state] ne \"disabled\"} {\n\
set Priv(buttonWindow) $w\n\
$w configure -relief sunken\n\
set Priv($w,prelief) sunken\n\
\n\
after cancel $Priv(afterId)\n\
set delay [$w cget -repeatdelay]\n\
set Priv(repeated) 0\n\
if {$delay > 0} {\n\
set Priv(afterId) [after $delay [list tk::ButtonAutoInvoke $w]]\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ButtonUp w {\n\
variable ::tk::Priv\n\
if {$w eq $Priv(buttonWindow)} {\n\
set Priv(buttonWindow) \"\"\n\
\n\
\n\
if {[info exists Priv($w,relief)]} {\n\
if {[info exists Priv($w,prelief)] && \\\n\
$Priv($w,prelief) eq [$w cget -relief]} {\n\
$w configure -relief $Priv($w,relief)\n\
}\n\
unset -nocomplain Priv($w,relief) Priv($w,prelief)\n\
}\n\
\n\
after cancel $Priv(afterId)\n\
\n\
if {$Priv(window) eq $w && [$w cget -state] ne \"disabled\"} {\n\
if { $Priv(repeated) == 0 } {\n\
uplevel #0 [list $w invoke]\n\
}\n\
}\n\
}\n\
}\n\
\n\
}\n\
\n\
if {[tk windowingsystem] eq \"aqua\"} {\n\
\n\
\n\
\n\
proc ::tk::ButtonEnter {w} {\n\
variable ::tk::Priv\n\
if {[$w cget -state] ne \"disabled\"} {\n\
\n\
\n\
if {$Priv(buttonWindow) eq $w} {\n\
$w configure -state active\n\
} elseif {[set over [$w cget -overrelief]] ne \"\"} {\n\
set Priv($w,relief)  [$w cget -relief]\n\
set Priv($w,prelief) $over\n\
$w configure -relief $over\n\
}\n\
}\n\
set Priv(window) $w\n\
}\n\
\n\
\n\
proc ::tk::ButtonLeave w {\n\
variable ::tk::Priv\n\
if {$w eq $Priv(buttonWindow)} {\n\
$w configure -state normal\n\
}\n\
\n\
\n\
if {[info exists Priv($w,relief)]} {\n\
if {[info exists Priv($w,prelief)] && \\\n\
$Priv($w,prelief) eq [$w cget -relief]} {\n\
$w configure -relief $Priv($w,relief)\n\
}\n\
unset -nocomplain Priv($w,relief) Priv($w,prelief)\n\
}\n\
\n\
set Priv(window) \"\"\n\
}\n\
\n\
\n\
proc ::tk::ButtonDown w {\n\
variable ::tk::Priv\n\
\n\
if {[$w cget -state] ne \"disabled\"} {\n\
set Priv(buttonWindow) $w\n\
$w configure -state active\n\
\n\
after cancel $Priv(afterId)\n\
set Priv(repeated) 0\n\
if { ![catch {$w cget -repeatdelay} delay] } {\n\
if {$delay > 0} {\n\
set Priv(afterId) [after $delay [list tk::ButtonAutoInvoke $w]]\n\
}\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ButtonUp w {\n\
variable ::tk::Priv\n\
if {$Priv(buttonWindow) eq $w} {\n\
set Priv(buttonWindow) \"\"\n\
$w configure -state normal\n\
\n\
\n\
if {[info exists Priv($w,relief)]} {\n\
if {[info exists Priv($w,prelief)] && \\\n\
$Priv($w,prelief) eq [$w cget -relief]} {\n\
$w configure -relief $Priv($w,relief)\n\
}\n\
unset -nocomplain Priv($w,relief) Priv($w,prelief)\n\
}\n\
\n\
after cancel $Priv(afterId)\n\
\n\
if {$Priv(window) eq $w && [$w cget -state] ne \"disabled\"} {\n\
if { $Priv(repeated) == 0 } {\n\
uplevel #0 [list $w invoke]\n\
}\n\
}\n\
}\n\
}\n\
\n\
}\n\
\n\
\n\
\n\
proc ::tk::ButtonInvoke w {\n\
if {[$w cget -state] ne \"disabled\"} {\n\
set oldRelief [$w cget -relief]\n\
set oldState [$w cget -state]\n\
$w configure -state active -relief sunken\n\
update idletasks\n\
after 100\n\
$w configure -state $oldState -relief $oldRelief\n\
uplevel #0 [list $w invoke]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ButtonAutoInvoke {w} {\n\
variable ::tk::Priv\n\
after cancel $Priv(afterId)\n\
set delay [$w cget -repeatinterval]\n\
if {$Priv(window) eq $w} {\n\
incr Priv(repeated)\n\
uplevel #0 [list $w invoke]\n\
}\n\
if {$delay > 0} {\n\
set Priv(afterId) [after $delay [list tk::ButtonAutoInvoke $w]]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::CheckRadioInvoke {w {cmd invoke}} {\n\
if {[$w cget -state] ne \"disabled\"} {\n\
uplevel #0 [list $w $cmd]\n\
}\n\
}\n\
\n\
\n\
\n\
proc ::tk::CheckInvoke {w} {\n\
variable ::tk::Priv\n\
if {[$w cget -state] ne \"disabled\"} {\n\
\n\
if {![$w cget -indicatoron] && [info exist Priv($w,selectcolor)]} {\n\
if {[$w cget -selectcolor] eq $Priv($w,aselectcolor)} {\n\
$w configure -selectcolor $Priv($w,selectcolor)\n\
} else {\n\
$w configure -selectcolor $Priv($w,aselectcolor)\n\
}\n\
}\n\
uplevel #0 [list $w invoke]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::CheckEnter {w} {\n\
variable ::tk::Priv\n\
if {[$w cget -state] ne \"disabled\"} {\n\
$w configure -state active\n\
\n\
\n\
set Priv($w,relief) [$w cget -relief]\n\
if {$Priv(buttonWindow) eq $w} {\n\
$w configure -relief sunken\n\
set Priv($w,prelief) sunken\n\
} elseif {[set over [$w cget -overrelief]] ne \"\"} {\n\
$w configure -relief $over\n\
set Priv($w,prelief) $over\n\
}\n\
\n\
\n\
if {![$w cget -indicatoron] && [$w cget -selectcolor] ne \"\"} {\n\
set Priv($w,selectcolor) [$w cget -selectcolor]\n\
lassign [winfo rgb $w [$w cget -selectcolor]]      r1 g1 b1\n\
lassign [winfo rgb $w [$w cget -activebackground]] r2 g2 b2\n\
set Priv($w,aselectcolor) \\\n\
[format \"#%04x%04x%04x\" [expr {($r1+$r2)/2}] \\\n\
[expr {($g1+$g2)/2}] [expr {($b1+$b2)/2}]]\n\
if {[uplevel #0 [list set [$w cget -variable]]]\n\
eq [$w cget -onvalue]} {\n\
$w configure -selectcolor $Priv($w,aselectcolor)\n\
}\n\
}\n\
}\n\
set Priv(window) $w\n\
}\n\
\n\
\n\
proc ::tk::CheckLeave {w} {\n\
variable ::tk::Priv\n\
if {[$w cget -state] ne \"disabled\"} {\n\
$w configure -state normal\n\
}\n\
\n\
\n\
if {![$w cget -indicatoron] && [info exist Priv($w,selectcolor)]} {\n\
$w configure -selectcolor $Priv($w,selectcolor)\n\
}\n\
unset -nocomplain Priv($w,selectcolor) Priv($w,aselectcolor)\n\
\n\
\n\
if {[info exists Priv($w,relief)]} {\n\
if {[info exists Priv($w,prelief)] && \\\n\
$Priv($w,prelief) eq [$w cget -relief]} {\n\
$w configure -relief $Priv($w,relief)\n\
}\n\
unset -nocomplain Priv($w,relief) Priv($w,prelief)\n\
}\n\
\n\
set Priv(window) \"\"\n\
}\n\
\n\
\n\
proc ::tk_dialog {w title text bitmap default args} {\n\
global tcl_platform\n\
variable ::tk::Priv\n\
\n\
if {[string is integer -strict $default]} {\n\
if {$default >= [llength $args]} {\n\
return -code error \"default button index greater than number of\\\n\
buttons specified for tk_dialog\"\n\
}\n\
} elseif {\"\" eq $default} {\n\
set default -1\n\
} else {\n\
set default [lsearch -exact $args $default]\n\
}\n\
\n\
set windowingsystem [tk windowingsystem]\n\
if {$windowingsystem eq \"aqua\"} {\n\
option add *Dialog*background systemDialogBackgroundActive widgetDefault\n\
option add *Dialog*Button.highlightBackground \\\n\
systemDialogBackgroundActive widgetDefault\n\
}\n\
\n\
\n\
destroy $w\n\
toplevel $w -class Dialog\n\
wm title $w $title\n\
wm iconname $w Dialog\n\
wm protocol $w WM_DELETE_WINDOW { }\n\
\n\
if {[winfo viewable [winfo toplevel [winfo parent $w]]] } {\n\
wm transient $w [winfo toplevel [winfo parent $w]]\n\
}\n\
\n\
if {$windowingsystem eq \"aqua\"} {\n\
::tk::unsupported::MacWindowStyle style $w moveableModal {}\n\
} elseif {$windowingsystem eq \"x11\"} {\n\
wm attributes $w -type dialog\n\
}\n\
\n\
frame $w.bot\n\
frame $w.top\n\
if {$windowingsystem eq \"x11\"} {\n\
$w.bot configure -relief raised -bd 1\n\
$w.top configure -relief raised -bd 1\n\
}\n\
pack $w.bot -side bottom -fill both\n\
pack $w.top -side top -fill both -expand 1\n\
grid anchor $w.bot center\n\
\n\
\n\
option add *Dialog.msg.wrapLength 3i widgetDefault\n\
option add *Dialog.msg.font TkCaptionFont widgetDefault\n\
\n\
label $w.msg -justify left -text $text\n\
pack $w.msg -in $w.top -side right -expand 1 -fill both -padx 3m -pady 3m\n\
if {$bitmap ne \"\"} {\n\
if {$windowingsystem eq \"aqua\" && $bitmap eq \"error\"} {\n\
set bitmap \"stop\"\n\
}\n\
label $w.bitmap -bitmap $bitmap\n\
pack $w.bitmap -in $w.top -side left -padx 3m -pady 3m\n\
}\n\
\n\
\n\
set i 0\n\
foreach but $args {\n\
button $w.button$i -text $but -command [list set ::tk::Priv(button) $i]\n\
if {$i == $default} {\n\
$w.button$i configure -default active\n\
} else {\n\
$w.button$i configure -default normal\n\
}\n\
grid $w.button$i -in $w.bot -column $i -row 0 -sticky ew \\\n\
-padx 10 -pady 4\n\
grid columnconfigure $w.bot $i\n\
if {$windowingsystem eq \"aqua\"} {\n\
set tmp [string tolower $but]\n\
if {$tmp eq \"ok\" || $tmp eq \"cancel\"} {\n\
grid columnconfigure $w.bot $i -minsize 90\n\
}\n\
grid configure $w.button$i -pady 7\n\
}\n\
incr i\n\
}\n\
\n\
\n\
if {$default >= 0} {\n\
bind $w <Return> [list $w.button$default invoke]\n\
}\n\
bind $w <<PrevWindow>> [list bind $w <Return> {[tk_focusPrev %W] invoke}]\n\
bind $w <Tab> [list bind $w <Return> {[tk_focusNext %W] invoke}]\n\
\n\
\n\
bind $w <Destroy> {set ::tk::Priv(button) -1}\n\
\n\
\n\
wm withdraw $w\n\
update idletasks\n\
set x [expr {[winfo screenwidth $w]/2 - [winfo reqwidth $w]/2 \\\n\
- [winfo vrootx [winfo parent $w]]}]\n\
set y [expr {[winfo screenheight $w]/2 - [winfo reqheight $w]/2 \\\n\
- [winfo vrooty [winfo parent $w]]}]\n\
if {$x < 0} {\n\
set x 0\n\
}\n\
if {$y < 0} {\n\
set y 0\n\
}\n\
wm maxsize $w [winfo screenwidth $w] [winfo screenheight $w]\n\
wm geometry $w +$x+$y\n\
wm deiconify $w\n\
\n\
tkwait visibility $w\n\
\n\
\n\
if {$default >= 0} {\n\
set focus $w.button$default\n\
} else {\n\
set focus $w\n\
}\n\
tk::SetFocusGrab $w $focus\n\
\n\
\n\
vwait ::tk::Priv(button)\n\
\n\
catch {\n\
\n\
bind $w <Destroy> {}\n\
}\n\
tk::RestoreFocusGrab $w $focus\n\
return $Priv(button)\n\
}\n\
\n\
\n\
bind Entry <<Cut>> {\n\
if {![catch {tk::EntryGetSelection %W} tk::Priv(data)]} {\n\
clipboard clear -displayof %W\n\
clipboard append -displayof %W $tk::Priv(data)\n\
%W delete sel.first sel.last\n\
unset tk::Priv(data)\n\
}\n\
}\n\
bind Entry <<Copy>> {\n\
if {![catch {tk::EntryGetSelection %W} tk::Priv(data)]} {\n\
clipboard clear -displayof %W\n\
clipboard append -displayof %W $tk::Priv(data)\n\
unset tk::Priv(data)\n\
}\n\
}\n\
bind Entry <<Paste>> {\n\
global tcl_platform\n\
catch {\n\
if {[tk windowingsystem] ne \"x11\"} {\n\
catch {\n\
%W delete sel.first sel.last\n\
}\n\
}\n\
%W insert insert [::tk::GetSelection %W CLIPBOARD]\n\
tk::EntrySeeInsert %W\n\
}\n\
}\n\
bind Entry <<Clear>> {\n\
catch { %W delete sel.first sel.last }\n\
}\n\
bind Entry <<PasteSelection>> {\n\
if {$tk_strictMotif || ![info exists tk::Priv(mouseMoved)]\n\
|| !$tk::Priv(mouseMoved)} {\n\
tk::EntryPaste %W %x\n\
}\n\
}\n\
\n\
bind Entry <<TraverseIn>> {\n\
%W selection range 0 end \n\
%W icursor end \n\
}\n\
\n\
\n\
bind Entry <1> {\n\
tk::EntryButton1 %W %x\n\
%W selection clear\n\
}\n\
bind Entry <B1-Motion> {\n\
set tk::Priv(x) %x\n\
tk::EntryMouseSelect %W %x\n\
}\n\
bind Entry <Double-1> {\n\
set tk::Priv(selectMode) word\n\
tk::EntryMouseSelect %W %x\n\
catch {%W icursor sel.last}\n\
}\n\
bind Entry <Triple-1> {\n\
set tk::Priv(selectMode) line\n\
tk::EntryMouseSelect %W %x\n\
catch {%W icursor sel.last}\n\
}\n\
bind Entry <Shift-1> {\n\
set tk::Priv(selectMode) char\n\
%W selection adjust @%x\n\
}\n\
bind Entry <Double-Shift-1>	{\n\
set tk::Priv(selectMode) word\n\
tk::EntryMouseSelect %W %x\n\
}\n\
bind Entry <Triple-Shift-1>	{\n\
set tk::Priv(selectMode) line\n\
tk::EntryMouseSelect %W %x\n\
}\n\
bind Entry <B1-Leave> {\n\
set tk::Priv(x) %x\n\
tk::EntryAutoScan %W\n\
}\n\
bind Entry <B1-Enter> {\n\
tk::CancelRepeat\n\
}\n\
bind Entry <ButtonRelease-1> {\n\
tk::CancelRepeat\n\
}\n\
bind Entry <Control-1> {\n\
%W icursor @%x\n\
}\n\
\n\
bind Entry <Left> {\n\
tk::EntrySetCursor %W [expr {[%W index insert] - 1}]\n\
}\n\
bind Entry <Right> {\n\
tk::EntrySetCursor %W [expr {[%W index insert] + 1}]\n\
}\n\
bind Entry <Shift-Left> {\n\
tk::EntryKeySelect %W [expr {[%W index insert] - 1}]\n\
tk::EntrySeeInsert %W\n\
}\n\
bind Entry <Shift-Right> {\n\
tk::EntryKeySelect %W [expr {[%W index insert] + 1}]\n\
tk::EntrySeeInsert %W\n\
}\n\
bind Entry <Control-Left> {\n\
tk::EntrySetCursor %W [tk::EntryPreviousWord %W insert]\n\
}\n\
bind Entry <Control-Right> {\n\
tk::EntrySetCursor %W [tk::EntryNextWord %W insert]\n\
}\n\
bind Entry <Shift-Control-Left> {\n\
tk::EntryKeySelect %W [tk::EntryPreviousWord %W insert]\n\
tk::EntrySeeInsert %W\n\
}\n\
bind Entry <Shift-Control-Right> {\n\
tk::EntryKeySelect %W [tk::EntryNextWord %W insert]\n\
tk::EntrySeeInsert %W\n\
}\n\
bind Entry <Home> {\n\
tk::EntrySetCursor %W 0\n\
}\n\
bind Entry <Shift-Home> {\n\
tk::EntryKeySelect %W 0\n\
tk::EntrySeeInsert %W\n\
}\n\
bind Entry <End> {\n\
tk::EntrySetCursor %W end\n\
}\n\
bind Entry <Shift-End> {\n\
tk::EntryKeySelect %W end\n\
tk::EntrySeeInsert %W\n\
}\n\
\n\
bind Entry <Delete> {\n\
if {[%W selection present]} {\n\
%W delete sel.first sel.last\n\
} else {\n\
%W delete insert\n\
}\n\
}\n\
bind Entry <BackSpace> {\n\
tk::EntryBackspace %W\n\
}\n\
\n\
bind Entry <Control-space> {\n\
%W selection from insert\n\
}\n\
bind Entry <Select> {\n\
%W selection from insert\n\
}\n\
bind Entry <Control-Shift-space> {\n\
%W selection adjust insert\n\
}\n\
bind Entry <Shift-Select> {\n\
%W selection adjust insert\n\
}\n\
bind Entry <Control-slash> {\n\
%W selection range 0 end\n\
}\n\
bind Entry <Control-backslash> {\n\
%W selection clear\n\
}\n\
bind Entry <KeyPress> {\n\
tk::CancelRepeat\n\
tk::EntryInsert %W %A\n\
}\n\
\n\
\n\
bind Entry <Alt-KeyPress> {# nothing}\n\
bind Entry <Meta-KeyPress> {# nothing}\n\
bind Entry <Control-KeyPress> {# nothing}\n\
bind Entry <Escape> {# nothing}\n\
bind Entry <Return> {# nothing}\n\
bind Entry <KP_Enter> {# nothing}\n\
bind Entry <Tab> {# nothing}\n\
if {[tk windowingsystem] eq \"aqua\"} {\n\
bind Entry <Command-KeyPress> {# nothing}\n\
}\n\
\n\
if {$tcl_platform(platform) ne \"windows\"} {\n\
bind Entry <Insert> {\n\
catch {tk::EntryInsert %W [::tk::GetSelection %W PRIMARY]}\n\
}\n\
}\n\
\n\
\n\
bind Entry <Control-a> {\n\
if {!$tk_strictMotif} {\n\
tk::EntrySetCursor %W 0\n\
}\n\
}\n\
bind Entry <Control-b> {\n\
if {!$tk_strictMotif} {\n\
tk::EntrySetCursor %W [expr {[%W index insert] - 1}]\n\
}\n\
}\n\
bind Entry <Control-d> {\n\
if {!$tk_strictMotif} {\n\
%W delete insert\n\
}\n\
}\n\
bind Entry <Control-e> {\n\
if {!$tk_strictMotif} {\n\
tk::EntrySetCursor %W end\n\
}\n\
}\n\
bind Entry <Control-f> {\n\
if {!$tk_strictMotif} {\n\
tk::EntrySetCursor %W [expr {[%W index insert] + 1}]\n\
}\n\
}\n\
bind Entry <Control-h> {\n\
if {!$tk_strictMotif} {\n\
tk::EntryBackspace %W\n\
}\n\
}\n\
bind Entry <Control-k> {\n\
if {!$tk_strictMotif} {\n\
%W delete insert end\n\
}\n\
}\n\
bind Entry <Control-t> {\n\
if {!$tk_strictMotif} {\n\
tk::EntryTranspose %W\n\
}\n\
}\n\
bind Entry <Meta-b> {\n\
if {!$tk_strictMotif} {\n\
tk::EntrySetCursor %W [tk::EntryPreviousWord %W insert]\n\
}\n\
}\n\
bind Entry <Meta-d> {\n\
if {!$tk_strictMotif} {\n\
%W delete insert [tk::EntryNextWord %W insert]\n\
}\n\
}\n\
bind Entry <Meta-f> {\n\
if {!$tk_strictMotif} {\n\
tk::EntrySetCursor %W [tk::EntryNextWord %W insert]\n\
}\n\
}\n\
bind Entry <Meta-BackSpace> {\n\
if {!$tk_strictMotif} {\n\
%W delete [tk::EntryPreviousWord %W insert] insert\n\
}\n\
}\n\
bind Entry <Meta-Delete> {\n\
if {!$tk_strictMotif} {\n\
%W delete [tk::EntryPreviousWord %W insert] insert\n\
}\n\
}\n\
\n\
\n\
bind Entry <2> {\n\
if {!$tk_strictMotif} {\n\
::tk::EntryScanMark %W %x\n\
}\n\
}\n\
bind Entry <B2-Motion> {\n\
if {!$tk_strictMotif} {\n\
::tk::EntryScanDrag %W %x\n\
}\n\
}\n\
\n\
\n\
proc ::tk::EntryClosestGap {w x} {\n\
set pos [$w index @$x]\n\
set bbox [$w bbox $pos]\n\
if {($x - [lindex $bbox 0]) < ([lindex $bbox 2]/2)} {\n\
return $pos\n\
}\n\
incr pos\n\
}\n\
\n\
\n\
proc ::tk::EntryButton1 {w x} {\n\
variable ::tk::Priv\n\
\n\
set Priv(selectMode) char\n\
set Priv(mouseMoved) 0\n\
set Priv(pressX) $x\n\
$w icursor [EntryClosestGap $w $x]\n\
$w selection from insert\n\
if {\"disabled\" ne [$w cget -state]} {\n\
focus $w\n\
}\n\
}\n\
\n\
\n\
proc ::tk::EntryMouseSelect {w x} {\n\
variable ::tk::Priv\n\
\n\
set cur [EntryClosestGap $w $x]\n\
set anchor [$w index anchor]\n\
if {($cur != $anchor) || (abs($Priv(pressX) - $x) >= 3)} {\n\
set Priv(mouseMoved) 1\n\
}\n\
switch $Priv(selectMode) {\n\
char {\n\
if {$Priv(mouseMoved)} {\n\
if {$cur < $anchor} {\n\
$w selection range $cur $anchor\n\
} elseif {$cur > $anchor} {\n\
$w selection range $anchor $cur\n\
} else {\n\
$w selection clear\n\
}\n\
}\n\
}\n\
word {\n\
if {$cur < [$w index anchor]} {\n\
set before [tcl_wordBreakBefore [$w get] $cur]\n\
set after [tcl_wordBreakAfter [$w get] [expr {$anchor-1}]]\n\
} else {\n\
set before [tcl_wordBreakBefore [$w get] $anchor]\n\
set after [tcl_wordBreakAfter [$w get] [expr {$cur - 1}]]\n\
}\n\
if {$before < 0} {\n\
set before 0\n\
}\n\
if {$after < 0} {\n\
set after end\n\
}\n\
$w selection range $before $after\n\
}\n\
line {\n\
$w selection range 0 end\n\
}\n\
}\n\
if {$Priv(mouseMoved)} {\n\
$w icursor $cur\n\
}\n\
update idletasks\n\
}\n\
\n\
\n\
proc ::tk::EntryPaste {w x} {\n\
$w icursor [EntryClosestGap $w $x]\n\
catch {$w insert insert [::tk::GetSelection $w PRIMARY]}\n\
if {\"disabled\" ne [$w cget -state]} {\n\
focus $w\n\
}\n\
}\n\
\n\
\n\
proc ::tk::EntryAutoScan {w} {\n\
variable ::tk::Priv\n\
set x $Priv(x)\n\
if {![winfo exists $w]} {\n\
return\n\
}\n\
if {$x >= [winfo width $w]} {\n\
$w xview scroll 2 units\n\
EntryMouseSelect $w $x\n\
} elseif {$x < 0} {\n\
$w xview scroll -2 units\n\
EntryMouseSelect $w $x\n\
}\n\
set Priv(afterId) [after 50 [list tk::EntryAutoScan $w]]\n\
}\n\
\n\
\n\
proc ::tk::EntryKeySelect {w new} {\n\
if {![$w selection present]} {\n\
$w selection from insert\n\
$w selection to $new\n\
} else {\n\
$w selection adjust $new\n\
}\n\
$w icursor $new\n\
}\n\
\n\
\n\
proc ::tk::EntryInsert {w s} {\n\
if {$s eq \"\"} {\n\
return\n\
}\n\
catch {\n\
set insert [$w index insert]\n\
if {([$w index sel.first] <= $insert)\n\
&& ([$w index sel.last] >= $insert)} {\n\
$w delete sel.first sel.last\n\
}\n\
}\n\
$w insert insert $s\n\
EntrySeeInsert $w\n\
}\n\
\n\
\n\
proc ::tk::EntryBackspace w {\n\
if {[$w selection present]} {\n\
$w delete sel.first sel.last\n\
} else {\n\
set x [expr {[$w index insert] - 1}]\n\
if {$x >= 0} {\n\
$w delete $x\n\
}\n\
if {[$w index @0] >= [$w index insert]} {\n\
set range [$w xview]\n\
set left [lindex $range 0]\n\
set right [lindex $range 1]\n\
$w xview moveto [expr {$left - ($right - $left)/2.0}]\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::EntrySeeInsert w {\n\
set c [$w index insert]\n\
if {($c < [$w index @0]) || ($c > [$w index @[winfo width $w]])} {\n\
$w xview $c\n\
}\n\
}\n\
\n\
\n\
proc ::tk::EntrySetCursor {w pos} {\n\
$w icursor $pos\n\
$w selection clear\n\
EntrySeeInsert $w\n\
}\n\
\n\
\n\
proc ::tk::EntryTranspose w {\n\
set i [$w index insert]\n\
if {$i < [$w index end]} {\n\
incr i\n\
}\n\
set first [expr {$i-2}]\n\
if {$first < 0} {\n\
return\n\
}\n\
set data [$w get]\n\
set new [string index $data [expr {$i-1}]][string index $data $first]\n\
$w delete $first $i\n\
$w insert insert $new\n\
EntrySeeInsert $w\n\
}\n\
\n\
\n\
if {$tcl_platform(platform) eq \"windows\"}  {\n\
proc ::tk::EntryNextWord {w start} {\n\
set pos [tcl_endOfWord [$w get] [$w index $start]]\n\
if {$pos >= 0} {\n\
set pos [tcl_startOfNextWord [$w get] $pos]\n\
}\n\
if {$pos < 0} {\n\
return end\n\
}\n\
return $pos\n\
}\n\
} else {\n\
proc ::tk::EntryNextWord {w start} {\n\
set pos [tcl_endOfWord [$w get] [$w index $start]]\n\
if {$pos < 0} {\n\
return end\n\
}\n\
return $pos\n\
}\n\
}\n\
\n\
\n\
proc ::tk::EntryPreviousWord {w start} {\n\
set pos [tcl_startOfPreviousWord [$w get] [$w index $start]]\n\
if {$pos < 0} {\n\
return 0\n\
}\n\
return $pos\n\
}\n\
\n\
\n\
proc ::tk::EntryScanMark {w x} {\n\
$w scan mark $x\n\
set ::tk::Priv(x) $x\n\
set ::tk::Priv(y) 0 ; # not used\n\
set ::tk::Priv(mouseMoved) 0\n\
}\n\
\n\
\n\
proc ::tk::EntryScanDrag {w x} {\n\
if {![info exists ::tk::Priv(x)]} { set ::tk::Priv(x) $x }\n\
if {abs($x-$::tk::Priv(x)) > 2} {\n\
set ::tk::Priv(mouseMoved) 1\n\
}\n\
$w scan dragto $x\n\
}\n\
\n\
\n\
proc ::tk::EntryGetSelection {w} {\n\
set entryString [string range [$w get] [$w index sel.first] \\\n\
[expr {[$w index sel.last] - 1}]]\n\
if {[$w cget -show] ne \"\"} {\n\
return [string repeat [string index [$w cget -show] 0] \\\n\
[string length $entryString]]\n\
}\n\
return $entryString\n\
}\n\
\n\
\n\
proc ::tk_focusNext w {\n\
set cur $w\n\
while {1} {\n\
\n\
\n\
set parent $cur\n\
set children [winfo children $cur]\n\
set i -1\n\
\n\
\n\
while {1} {\n\
incr i\n\
if {$i < [llength $children]} {\n\
set cur [lindex $children $i]\n\
if {[winfo toplevel $cur] eq $cur} {\n\
continue\n\
} else {\n\
break\n\
}\n\
}\n\
\n\
\n\
set cur $parent\n\
if {[winfo toplevel $cur] eq $cur} {\n\
break\n\
}\n\
set parent [winfo parent $parent]\n\
set children [winfo children $parent]\n\
set i [lsearch -exact $children $cur]\n\
}\n\
if {$w eq $cur || [tk::FocusOK $cur]} {\n\
return $cur\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk_focusPrev w {\n\
set cur $w\n\
while {1} {\n\
\n\
\n\
if {[winfo toplevel $cur] eq $cur}  {\n\
set parent $cur\n\
set children [winfo children $cur]\n\
set i [llength $children]\n\
} else {\n\
set parent [winfo parent $cur]\n\
set children [winfo children $parent]\n\
set i [lsearch -exact $children $cur]\n\
}\n\
\n\
\n\
while {$i > 0} {\n\
incr i -1\n\
set cur [lindex $children $i]\n\
if {[winfo toplevel $cur] eq $cur} {\n\
continue\n\
}\n\
set parent $cur\n\
set children [winfo children $parent]\n\
set i [llength $children]\n\
}\n\
set cur $parent\n\
if {$w eq $cur || [tk::FocusOK $cur]} {\n\
return $cur\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::FocusOK w {\n\
set code [catch {$w cget -takefocus} value]\n\
if {($code == 0) && ($value ne \"\")} {\n\
if {$value == 0} {\n\
return 0\n\
} elseif {$value == 1} {\n\
return [winfo viewable $w]\n\
} else {\n\
set value [uplevel #0 $value [list $w]]\n\
if {$value ne \"\"} {\n\
return $value\n\
}\n\
}\n\
}\n\
if {![winfo viewable $w]} {\n\
return 0\n\
}\n\
set code [catch {$w cget -state} value]\n\
if {($code == 0) && $value eq \"disabled\"} {\n\
return 0\n\
}\n\
regexp Key|Focus \"[bind $w] [bind [winfo class $w]]\"\n\
}\n\
\n\
\n\
proc ::tk_focusFollowsMouse {} {\n\
set old [bind all <Enter>]\n\
set script {\n\
if {\"%d\" eq \"NotifyAncestor\" || \"%d\" eq \"NotifyNonlinear\" \\\n\
|| \"%d\" eq \"NotifyInferior\"} {\n\
if {[tk::FocusOK %W]} {\n\
focus %W\n\
}\n\
}\n\
}\n\
if {$old ne \"\"} {\n\
bind all <Enter> \"$old; $script\"\n\
} else {\n\
bind all <Enter> $script\n\
}\n\
}\n\
\n\
\n\
\n\
\n\
bind Listbox <1> {\n\
if {[winfo exists %W]} {\n\
tk::ListboxBeginSelect %W [%W index @%x,%y] 1\n\
}\n\
}\n\
\n\
\n\
bind Listbox <Double-1> {\n\
}\n\
\n\
bind Listbox <B1-Motion> {\n\
set tk::Priv(x) %x\n\
set tk::Priv(y) %y\n\
tk::ListboxMotion %W [%W index @%x,%y]\n\
}\n\
bind Listbox <ButtonRelease-1> {\n\
tk::CancelRepeat\n\
%W activate @%x,%y\n\
}\n\
bind Listbox <Shift-1> {\n\
tk::ListboxBeginExtend %W [%W index @%x,%y]\n\
}\n\
bind Listbox <Control-1> {\n\
tk::ListboxBeginToggle %W [%W index @%x,%y]\n\
}\n\
bind Listbox <B1-Leave> {\n\
set tk::Priv(x) %x\n\
set tk::Priv(y) %y\n\
tk::ListboxAutoScan %W\n\
}\n\
bind Listbox <B1-Enter> {\n\
tk::CancelRepeat\n\
}\n\
\n\
bind Listbox <Up> {\n\
tk::ListboxUpDown %W -1\n\
}\n\
bind Listbox <Shift-Up> {\n\
tk::ListboxExtendUpDown %W -1\n\
}\n\
bind Listbox <Down> {\n\
tk::ListboxUpDown %W 1\n\
}\n\
bind Listbox <Shift-Down> {\n\
tk::ListboxExtendUpDown %W 1\n\
}\n\
bind Listbox <Left> {\n\
%W xview scroll -1 units\n\
}\n\
bind Listbox <Control-Left> {\n\
%W xview scroll -1 pages\n\
}\n\
bind Listbox <Right> {\n\
%W xview scroll 1 units\n\
}\n\
bind Listbox <Control-Right> {\n\
%W xview scroll 1 pages\n\
}\n\
bind Listbox <Prior> {\n\
%W yview scroll -1 pages\n\
%W activate @0,0\n\
}\n\
bind Listbox <Next> {\n\
%W yview scroll 1 pages\n\
%W activate @0,0\n\
}\n\
bind Listbox <Control-Prior> {\n\
%W xview scroll -1 pages\n\
}\n\
bind Listbox <Control-Next> {\n\
%W xview scroll 1 pages\n\
}\n\
bind Listbox <Home> {\n\
%W xview moveto 0\n\
}\n\
bind Listbox <End> {\n\
%W xview moveto 1\n\
}\n\
bind Listbox <Control-Home> {\n\
%W activate 0\n\
%W see 0\n\
%W selection clear 0 end\n\
%W selection set 0\n\
event generate %W <<ListboxSelect>>\n\
}\n\
bind Listbox <Shift-Control-Home> {\n\
tk::ListboxDataExtend %W 0\n\
}\n\
bind Listbox <Control-End> {\n\
%W activate end\n\
%W see end\n\
%W selection clear 0 end\n\
%W selection set end\n\
event generate %W <<ListboxSelect>>\n\
}\n\
bind Listbox <Shift-Control-End> {\n\
tk::ListboxDataExtend %W [%W index end]\n\
}\n\
bind Listbox <<Copy>> {\n\
if {[selection own -displayof %W] eq \"%W\"} {\n\
clipboard clear -displayof %W\n\
clipboard append -displayof %W [selection get -displayof %W]\n\
}\n\
}\n\
bind Listbox <space> {\n\
tk::ListboxBeginSelect %W [%W index active]\n\
}\n\
bind Listbox <Select> {\n\
tk::ListboxBeginSelect %W [%W index active]\n\
}\n\
bind Listbox <Control-Shift-space> {\n\
tk::ListboxBeginExtend %W [%W index active]\n\
}\n\
bind Listbox <Shift-Select> {\n\
tk::ListboxBeginExtend %W [%W index active]\n\
}\n\
bind Listbox <Escape> {\n\
tk::ListboxCancel %W\n\
}\n\
bind Listbox <Control-slash> {\n\
tk::ListboxSelectAll %W\n\
}\n\
bind Listbox <Control-backslash> {\n\
if {[%W cget -selectmode] ne \"browse\"} {\n\
%W selection clear 0 end\n\
event generate %W <<ListboxSelect>>\n\
}\n\
}\n\
\n\
\n\
bind Listbox <2> {\n\
%W scan mark %x %y\n\
}\n\
bind Listbox <B2-Motion> {\n\
%W scan dragto %x %y\n\
}\n\
\n\
\n\
if {[tk windowingsystem] eq \"aqua\"} {\n\
bind Listbox <MouseWheel> {\n\
%W yview scroll [expr {- (%D)}] units\n\
}\n\
bind Listbox <Option-MouseWheel> {\n\
%W yview scroll [expr {-10 * (%D)}] units\n\
}\n\
bind Listbox <Shift-MouseWheel> {\n\
%W xview scroll [expr {- (%D)}] units\n\
}\n\
bind Listbox <Shift-Option-MouseWheel> {\n\
%W xview scroll [expr {-10 * (%D)}] units\n\
}\n\
} else {\n\
bind Listbox <MouseWheel> {\n\
%W yview scroll [expr {- (%D / 120) * 4}] units\n\
}\n\
}\n\
\n\
if {\"x11\" eq [tk windowingsystem]} {\n\
bind Listbox <4> {\n\
if {!$tk_strictMotif} {\n\
%W yview scroll -5 units\n\
}\n\
}\n\
bind Listbox <5> {\n\
if {!$tk_strictMotif} {\n\
%W yview scroll 5 units\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ListboxBeginSelect {w el {focus 1}} {\n\
variable ::tk::Priv\n\
if {[$w cget -selectmode] eq \"multiple\"} {\n\
if {[$w selection includes $el]} {\n\
$w selection clear $el\n\
} else {\n\
$w selection set $el\n\
}\n\
} else {\n\
$w selection clear 0 end\n\
$w selection set $el\n\
$w selection anchor $el\n\
set Priv(listboxSelection) {}\n\
set Priv(listboxPrev) $el\n\
}\n\
event generate $w <<ListboxSelect>>\n\
if {$focus && [winfo exists $w] && [$w cget -state] eq \"normal\"} {\n\
focus $w\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ListboxMotion {w el} {\n\
variable ::tk::Priv\n\
if {$el == $Priv(listboxPrev)} {\n\
return\n\
}\n\
set anchor [$w index anchor]\n\
switch [$w cget -selectmode] {\n\
browse {\n\
$w selection clear 0 end\n\
$w selection set $el\n\
set Priv(listboxPrev) $el\n\
event generate $w <<ListboxSelect>>\n\
}\n\
extended {\n\
set i $Priv(listboxPrev)\n\
if {$i eq \"\"} {\n\
set i $el\n\
$w selection set $el\n\
}\n\
if {[$w selection includes anchor]} {\n\
$w selection clear $i $el\n\
$w selection set anchor $el\n\
} else {\n\
$w selection clear $i $el\n\
$w selection clear anchor $el\n\
}\n\
if {![info exists Priv(listboxSelection)]} {\n\
set Priv(listboxSelection) [$w curselection]\n\
}\n\
while {($i < $el) && ($i < $anchor)} {\n\
if {[lsearch $Priv(listboxSelection) $i] >= 0} {\n\
$w selection set $i\n\
}\n\
incr i\n\
}\n\
while {($i > $el) && ($i > $anchor)} {\n\
if {[lsearch $Priv(listboxSelection) $i] >= 0} {\n\
$w selection set $i\n\
}\n\
incr i -1\n\
}\n\
set Priv(listboxPrev) $el\n\
event generate $w <<ListboxSelect>>\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ListboxBeginExtend {w el} {\n\
if {[$w cget -selectmode] eq \"extended\"} {\n\
if {[$w selection includes anchor]} {\n\
ListboxMotion $w $el\n\
} else {\n\
ListboxBeginSelect $w $el\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ListboxBeginToggle {w el} {\n\
variable ::tk::Priv\n\
if {[$w cget -selectmode] eq \"extended\"} {\n\
set Priv(listboxSelection) [$w curselection]\n\
set Priv(listboxPrev) $el\n\
$w selection anchor $el\n\
if {[$w selection includes $el]} {\n\
$w selection clear $el\n\
} else {\n\
$w selection set $el\n\
}\n\
event generate $w <<ListboxSelect>>\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ListboxAutoScan {w} {\n\
variable ::tk::Priv\n\
if {![winfo exists $w]} return\n\
set x $Priv(x)\n\
set y $Priv(y)\n\
if {$y >= [winfo height $w]} {\n\
$w yview scroll 1 units\n\
} elseif {$y < 0} {\n\
$w yview scroll -1 units\n\
} elseif {$x >= [winfo width $w]} {\n\
$w xview scroll 2 units\n\
} elseif {$x < 0} {\n\
$w xview scroll -2 units\n\
} else {\n\
return\n\
}\n\
ListboxMotion $w [$w index @$x,$y]\n\
set Priv(afterId) [after 50 [list tk::ListboxAutoScan $w]]\n\
}\n\
\n\
\n\
proc ::tk::ListboxUpDown {w amount} {\n\
variable ::tk::Priv\n\
$w activate [expr {[$w index active] + $amount}]\n\
$w see active\n\
switch [$w cget -selectmode] {\n\
browse {\n\
$w selection clear 0 end\n\
$w selection set active\n\
event generate $w <<ListboxSelect>>\n\
}\n\
extended {\n\
$w selection clear 0 end\n\
$w selection set active\n\
$w selection anchor active\n\
set Priv(listboxPrev) [$w index active]\n\
set Priv(listboxSelection) {}\n\
event generate $w <<ListboxSelect>>\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ListboxExtendUpDown {w amount} {\n\
variable ::tk::Priv\n\
if {[$w cget -selectmode] ne \"extended\"} {\n\
return\n\
}\n\
set active [$w index active]\n\
if {![info exists Priv(listboxSelection)]} {\n\
$w selection set $active\n\
set Priv(listboxSelection) [$w curselection]\n\
}\n\
$w activate [expr {$active + $amount}]\n\
$w see active\n\
ListboxMotion $w [$w index active]\n\
}\n\
\n\
\n\
proc ::tk::ListboxDataExtend {w el} {\n\
set mode [$w cget -selectmode]\n\
if {$mode eq \"extended\"} {\n\
$w activate $el\n\
$w see $el\n\
if {[$w selection includes anchor]} {\n\
ListboxMotion $w $el\n\
}\n\
} elseif {$mode eq \"multiple\"} {\n\
$w activate $el\n\
$w see $el\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ListboxCancel w {\n\
variable ::tk::Priv\n\
if {[$w cget -selectmode] ne \"extended\"} {\n\
return\n\
}\n\
set first [$w index anchor]\n\
set last $Priv(listboxPrev)\n\
if {$last eq \"\"} {\n\
return\n\
}\n\
if {$first > $last} {\n\
set tmp $first\n\
set first $last\n\
set last $tmp\n\
}\n\
$w selection clear $first $last\n\
while {$first <= $last} {\n\
if {[lsearch $Priv(listboxSelection) $first] >= 0} {\n\
$w selection set $first\n\
}\n\
incr first\n\
}\n\
event generate $w <<ListboxSelect>>\n\
}\n\
\n\
\n\
proc ::tk::ListboxSelectAll w {\n\
set mode [$w cget -selectmode]\n\
if {$mode eq \"single\" || $mode eq \"browse\"} {\n\
$w selection clear 0 end\n\
$w selection set active\n\
} else {\n\
$w selection set 0 end\n\
}\n\
event generate $w <<ListboxSelect>>\n\
}\n\
\n\
\n\
\n\
\n\
bind Menubutton <FocusIn> {}\n\
bind Menubutton <Enter> {\n\
tk::MbEnter %W\n\
}\n\
bind Menubutton <Leave> {\n\
tk::MbLeave %W\n\
}\n\
bind Menubutton <1> {\n\
if {$tk::Priv(inMenubutton) ne \"\"} {\n\
tk::MbPost $tk::Priv(inMenubutton) %X %Y\n\
}\n\
}\n\
bind Menubutton <Motion> {\n\
tk::MbMotion %W up %X %Y\n\
}\n\
bind Menubutton <B1-Motion> {\n\
tk::MbMotion %W down %X %Y\n\
}\n\
bind Menubutton <ButtonRelease-1> {\n\
tk::MbButtonUp %W\n\
}\n\
bind Menubutton <space> {\n\
tk::MbPost %W\n\
tk::MenuFirstEntry [%W cget -menu]\n\
}\n\
\n\
\n\
bind Menu <FocusIn> {}\n\
\n\
bind Menu <Enter> {\n\
set tk::Priv(window) %W\n\
if {[%W cget -type] eq \"tearoff\"} {\n\
if {\"%m\" ne \"NotifyUngrab\"} {\n\
if {[tk windowingsystem] eq \"x11\"} {\n\
tk_menuSetFocus %W\n\
}\n\
}\n\
}\n\
tk::MenuMotion %W %x %y %s\n\
}\n\
\n\
bind Menu <Leave> {\n\
tk::MenuLeave %W %X %Y %s\n\
}\n\
bind Menu <Motion> {\n\
tk::MenuMotion %W %x %y %s\n\
}\n\
bind Menu <ButtonPress> {\n\
tk::MenuButtonDown %W\n\
}\n\
bind Menu <ButtonRelease> {\n\
tk::MenuInvoke %W 1\n\
}\n\
bind Menu <space> {\n\
tk::MenuInvoke %W 0\n\
}\n\
bind Menu <Return> {\n\
tk::MenuInvoke %W 0\n\
}\n\
bind Menu <Escape> {\n\
tk::MenuEscape %W\n\
}\n\
bind Menu <Left> {\n\
tk::MenuLeftArrow %W\n\
}\n\
bind Menu <Right> {\n\
tk::MenuRightArrow %W\n\
}\n\
bind Menu <Up> {\n\
tk::MenuUpArrow %W\n\
}\n\
bind Menu <Down> {\n\
tk::MenuDownArrow %W\n\
}\n\
bind Menu <KeyPress> {\n\
tk::TraverseWithinMenu %W %A\n\
}\n\
\n\
\n\
if {[tk windowingsystem] eq \"x11\"} {\n\
bind all <Alt-KeyPress> {\n\
tk::TraverseToMenu %W %A\n\
}\n\
\n\
bind all <F10> {\n\
tk::FirstMenu %W\n\
}\n\
} else {\n\
bind Menubutton <Alt-KeyPress> {\n\
tk::TraverseToMenu %W %A\n\
}\n\
\n\
bind Menubutton <F10> {\n\
tk::FirstMenu %W\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MbEnter w {\n\
variable ::tk::Priv\n\
\n\
if {$Priv(inMenubutton) ne \"\"} {\n\
MbLeave $Priv(inMenubutton)\n\
}\n\
set Priv(inMenubutton) $w\n\
if {[$w cget -state] ne \"disabled\" && [tk windowingsystem] ne \"aqua\"} {\n\
$w configure -state active\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MbLeave w {\n\
variable ::tk::Priv\n\
\n\
set Priv(inMenubutton) {}\n\
if {![winfo exists $w]} {\n\
return\n\
}\n\
if {[$w cget -state] eq \"active\" && [tk windowingsystem] ne \"aqua\"} {\n\
$w configure -state normal\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MbPost {w {x {}} {y {}}} {\n\
global errorInfo\n\
variable ::tk::Priv\n\
global tcl_platform\n\
\n\
if {[$w cget -state] eq \"disabled\" || $w eq $Priv(postedMb)} {\n\
return\n\
}\n\
set menu [$w cget -menu]\n\
if {$menu eq \"\"} {\n\
return\n\
}\n\
set tearoff [expr {[tk windowingsystem] eq \"x11\" \\\n\
|| [$menu cget -type] eq \"tearoff\"}]\n\
if {[string first $w $menu] != 0} {\n\
error \"can't post $menu:  it isn't a descendant of $w (this is a new requirement in Tk versions 3.0 and later)\"\n\
}\n\
set cur $Priv(postedMb)\n\
if {$cur ne \"\"} {\n\
MenuUnpost {}\n\
}\n\
if {$::tk_strictMotif} {\n\
set Priv(cursor) [$w cget -cursor]\n\
$w configure -cursor arrow\n\
}\n\
if {[tk windowingsystem] ne \"aqua\"} {\n\
set Priv(relief) [$w cget -relief]\n\
$w configure -relief raised\n\
} else {\n\
$w configure -state active\n\
}\n\
\n\
set Priv(postedMb) $w\n\
set Priv(focus) [focus]\n\
$menu activate none\n\
GenerateMenuSelect $menu\n\
\n\
\n\
update idletasks\n\
if {[catch {\n\
switch [$w cget -direction] {\n\
above {\n\
set x [winfo rootx $w]\n\
set y [expr {[winfo rooty $w] - [winfo reqheight $menu]}]\n\
if {$y < 0} {\n\
set y [expr {[winfo rooty $w] + [winfo height $w]}]\n\
}\n\
PostOverPoint $menu $x $y\n\
}\n\
below {\n\
set x [winfo rootx $w]\n\
set y [expr {[winfo rooty $w] + [winfo height $w]}]\n\
set mh [winfo reqheight $menu]\n\
if {($y + $mh) > [winfo screenheight $w]} {\n\
set y [expr {[winfo rooty $w] - $mh}]\n\
}\n\
PostOverPoint $menu $x $y\n\
}\n\
left {\n\
set x [expr {[winfo rootx $w] - [winfo reqwidth $menu]}]\n\
set y [expr {(2 * [winfo rooty $w] + [winfo height $w]) / 2}]\n\
set entry [MenuFindName $menu [$w cget -text]]\n\
if {[$w cget -indicatoron] && $entry ne \"\"} {\n\
if {$entry == [$menu index last]} {\n\
incr y [expr {-([$menu yposition $entry] \\\n\
+ [winfo reqheight $menu])/2}]\n\
} else {\n\
incr y [expr {-([$menu yposition $entry] \\\n\
+ [$menu yposition [expr {$entry+1}]])/2}]\n\
}\n\
}\n\
PostOverPoint $menu $x $y\n\
if {$entry ne \"\" \\\n\
&& [$menu entrycget $entry -state] ne \"disabled\"} {\n\
$menu activate $entry\n\
GenerateMenuSelect $menu\n\
}\n\
}\n\
right {\n\
set x [expr {[winfo rootx $w] + [winfo width $w]}]\n\
set y [expr {(2 * [winfo rooty $w] + [winfo height $w]) / 2}]\n\
set entry [MenuFindName $menu [$w cget -text]]\n\
if {[$w cget -indicatoron] && $entry ne \"\"} {\n\
if {$entry == [$menu index last]} {\n\
incr y [expr {-([$menu yposition $entry] \\\n\
+ [winfo reqheight $menu])/2}]\n\
} else {\n\
incr y [expr {-([$menu yposition $entry] \\\n\
+ [$menu yposition [expr {$entry+1}]])/2}]\n\
}\n\
}\n\
PostOverPoint $menu $x $y\n\
if {$entry ne \"\" \\\n\
&& [$menu entrycget $entry -state] ne \"disabled\"} {\n\
$menu activate $entry\n\
GenerateMenuSelect $menu\n\
}\n\
}\n\
default {\n\
if {[$w cget -indicatoron]} {\n\
if {$y eq \"\"} {\n\
set x [expr {[winfo rootx $w] + [winfo width $w]/2}]\n\
set y [expr {[winfo rooty $w] + [winfo height $w]/2}]\n\
}\n\
PostOverPoint $menu $x $y [MenuFindName $menu [$w cget -text]]\n\
} else {\n\
PostOverPoint $menu [winfo rootx $w] [expr {[winfo rooty $w]+[winfo height $w]}]\n\
}\n\
}\n\
}\n\
} msg]} {\n\
\n\
set savedInfo $errorInfo\n\
MenuUnpost {}\n\
error $msg $savedInfo\n\
\n\
}\n\
\n\
set Priv(tearoff) $tearoff\n\
if {$tearoff != 0} {\n\
focus $menu\n\
if {[winfo viewable $w]} {\n\
SaveGrabInfo $w\n\
grab -global $w\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MenuUnpost menu {\n\
global tcl_platform\n\
variable ::tk::Priv\n\
set mb $Priv(postedMb)\n\
\n\
\n\
catch {focus $Priv(focus)}\n\
set Priv(focus) \"\"\n\
\n\
\n\
after cancel [array get Priv menuActivatedTimer]\n\
unset -nocomplain Priv(menuActivated)\n\
after cancel [array get Priv menuDeactivatedTimer]\n\
unset -nocomplain Priv(menuDeactivated)\n\
\n\
catch {\n\
if {$mb ne \"\"} {\n\
set menu [$mb cget -menu]\n\
$menu unpost\n\
set Priv(postedMb) {}\n\
if {$::tk_strictMotif} {\n\
$mb configure -cursor $Priv(cursor)\n\
}\n\
if {[tk windowingsystem] ne \"aqua\"} {\n\
$mb configure -relief $Priv(relief)\n\
} else {\n\
$mb configure -state normal\n\
}\n\
} elseif {$Priv(popup) ne \"\"} {\n\
$Priv(popup) unpost\n\
set Priv(popup) {}\n\
} elseif {[$menu cget -type] ne \"menubar\" \\\n\
&& [$menu cget -type] ne \"tearoff\"} {\n\
\n\
while {1} {\n\
set parent [winfo parent $menu]\n\
if {[winfo class $parent] ne \"Menu\" \\\n\
|| ![winfo ismapped $parent]} {\n\
break\n\
}\n\
$parent activate none\n\
$parent postcascade none\n\
GenerateMenuSelect $parent\n\
set type [$parent cget -type]\n\
if {$type eq \"menubar\" || $type eq \"tearoff\"} {\n\
break\n\
}\n\
set menu $parent\n\
}\n\
if {[$menu cget -type] ne \"menubar\"} {\n\
$menu unpost\n\
}\n\
}\n\
}\n\
\n\
if {($Priv(tearoff) != 0) || $Priv(menuBar) ne \"\"} {\n\
if {$menu ne \"\"} {\n\
set grab [grab current $menu]\n\
if {$grab ne \"\"} {\n\
grab release $grab\n\
}\n\
}\n\
RestoreOldGrab\n\
if {$Priv(menuBar) ne \"\"} {\n\
if {$::tk_strictMotif} {\n\
$Priv(menuBar) configure -cursor $Priv(cursor)\n\
}\n\
set Priv(menuBar) {}\n\
}\n\
if {[tk windowingsystem] ne \"x11\"} {\n\
set Priv(tearoff) 0\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MbMotion {w upDown rootx rooty} {\n\
variable ::tk::Priv\n\
\n\
if {$Priv(inMenubutton) eq $w} {\n\
return\n\
}\n\
set new [winfo containing $rootx $rooty]\n\
if {$new ne $Priv(inMenubutton) \\\n\
&& ($new eq \"\" || [winfo toplevel $new] eq [winfo toplevel $w])} {\n\
if {$Priv(inMenubutton) ne \"\"} {\n\
MbLeave $Priv(inMenubutton)\n\
}\n\
if {$new ne \"\" \\\n\
&& [winfo class $new] eq \"Menubutton\" \\\n\
&& ([$new cget -indicatoron] == 0) \\\n\
&& ([$w cget -indicatoron] == 0)} {\n\
if {$upDown eq \"down\"} {\n\
MbPost $new $rootx $rooty\n\
} else {\n\
MbEnter $new\n\
}\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MbButtonUp w {\n\
variable ::tk::Priv\n\
global tcl_platform\n\
\n\
set menu [$w cget -menu]\n\
set tearoff [expr {[tk windowingsystem] eq \"x11\" || \\\n\
($menu ne \"\" && [$menu cget -type] eq \"tearoff\")}]\n\
if {($tearoff != 0) && $Priv(postedMb) eq $w \\\n\
&& $Priv(inMenubutton) eq $w} {\n\
MenuFirstEntry [$Priv(postedMb) cget -menu]\n\
} else {\n\
MenuUnpost {}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MenuMotion {menu x y state} {\n\
variable ::tk::Priv\n\
if {$menu eq $Priv(window)} {\n\
set activeindex [$menu index active]\n\
if {[$menu cget -type] eq \"menubar\"} {\n\
if {[info exists Priv(focus)] && $menu ne $Priv(focus)} {\n\
$menu activate @$x,$y\n\
GenerateMenuSelect $menu\n\
}\n\
} else {\n\
$menu activate @$x,$y\n\
GenerateMenuSelect $menu\n\
}\n\
set index [$menu index @$x,$y]\n\
if {[info exists Priv(menuActivated)] \\\n\
&& $index ne \"none\" \\\n\
&& $index ne $activeindex} {\n\
set mode [option get $menu clickToFocus ClickToFocus]\n\
if {[string is false $mode]} {\n\
set delay [expr {[$menu cget -type] eq \"menubar\" ? 0 : 50}]\n\
if {[$menu type $index] eq \"cascade\"} {\n\
set Priv(menuActivatedTimer) \\\n\
[after $delay [list $menu postcascade active]]\n\
} else {\n\
set Priv(menuDeactivatedTimer) \\\n\
[after $delay [list $menu postcascade none]]\n\
}\n\
}\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MenuButtonDown menu {\n\
variable ::tk::Priv\n\
global tcl_platform\n\
\n\
if {![winfo viewable $menu]} {\n\
return\n\
}\n\
$menu postcascade active\n\
if {$Priv(postedMb) ne \"\" && [winfo viewable $Priv(postedMb)]} {\n\
grab -global $Priv(postedMb)\n\
} else {\n\
while {[$menu cget -type] eq \"normal\" \\\n\
&& [winfo class [winfo parent $menu]] eq \"Menu\" \\\n\
&& [winfo ismapped [winfo parent $menu]]} {\n\
set menu [winfo parent $menu]\n\
}\n\
\n\
if {$Priv(menuBar) eq {}} {\n\
set Priv(menuBar) $menu\n\
if {$::tk_strictMotif} {\n\
set Priv(cursor) [$menu cget -cursor]\n\
$menu configure -cursor arrow\n\
}\n\
if {[$menu type active] eq \"cascade\"} {\n\
set Priv(menuActivated) 1\n\
}\n\
}\n\
\n\
\n\
if {$menu ne [grab current $menu]} {\n\
SaveGrabInfo $menu\n\
}\n\
\n\
\n\
if {[tk windowingsystem] eq \"x11\"} {\n\
grab -global $menu\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MenuLeave {menu rootx rooty state} {\n\
variable ::tk::Priv\n\
set Priv(window) {}\n\
if {[$menu index active] eq \"none\"} {\n\
return\n\
}\n\
if {[$menu type active] eq \"cascade\" \\\n\
&& [winfo containing $rootx $rooty] eq \\\n\
[$menu entrycget active -menu]} {\n\
return\n\
}\n\
$menu activate none\n\
GenerateMenuSelect $menu\n\
}\n\
\n\
\n\
proc ::tk::MenuInvoke {w buttonRelease} {\n\
variable ::tk::Priv\n\
\n\
if {$buttonRelease && $Priv(window) eq \"\"} {\n\
\n\
$w postcascade none\n\
$w activate none\n\
event generate $w <<MenuSelect>>\n\
MenuUnpost $w\n\
return\n\
}\n\
if {[$w type active] eq \"cascade\"} {\n\
$w postcascade active\n\
set menu [$w entrycget active -menu]\n\
MenuFirstEntry $menu\n\
} elseif {[$w type active] eq \"tearoff\"} {\n\
::tk::TearOffMenu $w\n\
MenuUnpost $w\n\
} elseif {[$w cget -type] eq \"menubar\"} {\n\
$w postcascade none\n\
set active [$w index active]\n\
set isCascade [string equal [$w type $active] \"cascade\"]\n\
\n\
\n\
if { $isCascade } {\n\
$w activate none\n\
event generate $w <<MenuSelect>>\n\
}\n\
\n\
MenuUnpost $w\n\
\n\
\n\
if { !$isCascade } {\n\
uplevel #0 [list $w invoke $active]\n\
}\n\
} else {\n\
set active [$w index active]\n\
if {$Priv(popup) eq \"\" || $active ne \"none\"} {\n\
MenuUnpost $w\n\
}\n\
uplevel #0 [list $w invoke active]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MenuEscape menu {\n\
set parent [winfo parent $menu]\n\
if {[winfo class $parent] ne \"Menu\"} {\n\
MenuUnpost $menu\n\
} elseif {[$parent cget -type] eq \"menubar\"} {\n\
MenuUnpost $menu\n\
RestoreOldGrab\n\
} else {\n\
MenuNextMenu $menu left\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MenuUpArrow {menu} {\n\
if {[$menu cget -type] eq \"menubar\"} {\n\
MenuNextMenu $menu left\n\
} else {\n\
MenuNextEntry $menu -1\n\
}\n\
}\n\
\n\
proc ::tk::MenuDownArrow {menu} {\n\
if {[$menu cget -type] eq \"menubar\"} {\n\
MenuNextMenu $menu right\n\
} else {\n\
MenuNextEntry $menu 1\n\
}\n\
}\n\
\n\
proc ::tk::MenuLeftArrow {menu} {\n\
if {[$menu cget -type] eq \"menubar\"} {\n\
MenuNextEntry $menu -1\n\
} else {\n\
MenuNextMenu $menu left\n\
}\n\
}\n\
\n\
proc ::tk::MenuRightArrow {menu} {\n\
if {[$menu cget -type] eq \"menubar\"} {\n\
MenuNextEntry $menu 1\n\
} else {\n\
MenuNextMenu $menu right\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MenuNextMenu {menu direction} {\n\
variable ::tk::Priv\n\
\n\
\n\
if {$direction eq \"right\"} {\n\
set count 1\n\
set parent [winfo parent $menu]\n\
set class [winfo class $parent]\n\
if {[$menu type active] eq \"cascade\"} {\n\
$menu postcascade active\n\
set m2 [$menu entrycget active -menu]\n\
if {$m2 ne \"\"} {\n\
MenuFirstEntry $m2\n\
}\n\
return\n\
} else {\n\
set parent [winfo parent $menu]\n\
while {$parent ne \".\"} {\n\
if {[winfo class $parent] eq \"Menu\" \\\n\
&& [$parent cget -type] eq \"menubar\"} {\n\
tk_menuSetFocus $parent\n\
MenuNextEntry $parent 1\n\
return\n\
}\n\
set parent [winfo parent $parent]\n\
}\n\
}\n\
} else {\n\
set count -1\n\
set m2 [winfo parent $menu]\n\
if {[winfo class $m2] eq \"Menu\"} {\n\
$menu activate none\n\
GenerateMenuSelect $menu\n\
tk_menuSetFocus $m2\n\
\n\
$m2 postcascade none\n\
\n\
if {[$m2 cget -type] ne \"menubar\"} {\n\
return\n\
}\n\
}\n\
}\n\
\n\
\n\
set m2 [winfo parent $menu]\n\
if {[winfo class $m2] eq \"Menu\" && [$m2 cget -type] eq \"menubar\"} {\n\
tk_menuSetFocus $m2\n\
MenuNextEntry $m2 -1\n\
return\n\
}\n\
\n\
set w $Priv(postedMb)\n\
if {$w eq \"\"} {\n\
return\n\
}\n\
set buttons [winfo children [winfo parent $w]]\n\
set length [llength $buttons]\n\
set i [expr {[lsearch -exact $buttons $w] + $count}]\n\
while {1} {\n\
while {$i < 0} {\n\
incr i $length\n\
}\n\
while {$i >= $length} {\n\
incr i -$length\n\
}\n\
set mb [lindex $buttons $i]\n\
if {[winfo class $mb] eq \"Menubutton\" \\\n\
&& [$mb cget -state] ne \"disabled\" \\\n\
&& [$mb cget -menu] ne \"\" \\\n\
&& [[$mb cget -menu] index last] ne \"none\"} {\n\
break\n\
}\n\
if {$mb eq $w} {\n\
return\n\
}\n\
incr i $count\n\
}\n\
MbPost $mb\n\
MenuFirstEntry [$mb cget -menu]\n\
}\n\
\n\
\n\
proc ::tk::MenuNextEntry {menu count} {\n\
if {[$menu index last] eq \"none\"} {\n\
return\n\
}\n\
set length [expr {[$menu index last]+1}]\n\
set quitAfter $length\n\
set active [$menu index active]\n\
if {$active eq \"none\"} {\n\
set i 0\n\
} else {\n\
set i [expr {$active + $count}]\n\
}\n\
while {1} {\n\
if {$quitAfter <= 0} {\n\
\n\
return\n\
}\n\
while {$i < 0} {\n\
incr i $length\n\
}\n\
while {$i >= $length} {\n\
incr i -$length\n\
}\n\
if {[catch {$menu entrycget $i -state} state] == 0} {\n\
if {$state ne \"disabled\" && \\\n\
($i!=0 || [$menu cget -type] ne \"tearoff\" \\\n\
|| [$menu type 0] ne \"tearoff\")} {\n\
break\n\
}\n\
}\n\
if {$i == $active} {\n\
return\n\
}\n\
incr i $count\n\
incr quitAfter -1\n\
}\n\
$menu activate $i\n\
GenerateMenuSelect $menu\n\
\n\
if {[$menu type $i] eq \"cascade\" && [$menu cget -type] eq \"menubar\"} {\n\
set cascade [$menu entrycget $i -menu]\n\
if {$cascade ne \"\"} {\n\
$menu postcascade $i\n\
MenuFirstEntry $cascade\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MenuFind {w char} {\n\
set char [string tolower $char]\n\
set windowlist [winfo child $w]\n\
\n\
foreach child $windowlist {\n\
if {[winfo toplevel $w] ne [winfo toplevel $child]} {\n\
continue\n\
}\n\
if {[winfo class $child] eq \"Menu\" && \\\n\
[$child cget -type] eq \"menubar\"} {\n\
if {$char eq \"\"} {\n\
return $child\n\
}\n\
set last [$child index last]\n\
for {set i [$child cget -tearoff]} {$i <= $last} {incr i} {\n\
if {[$child type $i] eq \"separator\"} {\n\
continue\n\
}\n\
set char2 [string index [$child entrycget $i -label] \\\n\
[$child entrycget $i -underline]]\n\
if {$char eq [string tolower $char2] || $char eq \"\"} {\n\
if {[$child entrycget $i -state] ne \"disabled\"} {\n\
return $child\n\
}\n\
}\n\
}\n\
}\n\
}\n\
\n\
foreach child $windowlist {\n\
if {[winfo toplevel $w] ne [winfo toplevel $child]} {\n\
continue\n\
}\n\
switch -- [winfo class $child] {\n\
Menubutton {\n\
set char2 [string index [$child cget -text] \\\n\
[$child cget -underline]]\n\
if {$char eq [string tolower $char2] || $char eq \"\"} {\n\
if {[$child cget -state] ne \"disabled\"} {\n\
return $child\n\
}\n\
}\n\
}\n\
\n\
default {\n\
set match [MenuFind $child $char]\n\
if {$match ne \"\"} {\n\
return $match\n\
}\n\
}\n\
}\n\
}\n\
return {}\n\
}\n\
\n\
\n\
proc ::tk::TraverseToMenu {w char} {\n\
variable ::tk::Priv\n\
if {$char eq \"\"} {\n\
return\n\
}\n\
while {[winfo class $w] eq \"Menu\"} {\n\
if {[$w cget -type] ne \"menubar\" && $Priv(postedMb) eq \"\"} {\n\
return\n\
}\n\
if {[$w cget -type] eq \"menubar\"} {\n\
break\n\
}\n\
set w [winfo parent $w]\n\
}\n\
set w [MenuFind [winfo toplevel $w] $char]\n\
if {$w ne \"\"} {\n\
if {[winfo class $w] eq \"Menu\"} {\n\
tk_menuSetFocus $w\n\
set Priv(window) $w\n\
SaveGrabInfo $w\n\
grab -global $w\n\
TraverseWithinMenu $w $char\n\
} else {\n\
MbPost $w\n\
MenuFirstEntry [$w cget -menu]\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::FirstMenu w {\n\
variable ::tk::Priv\n\
set w [MenuFind [winfo toplevel $w] \"\"]\n\
if {$w ne \"\"} {\n\
if {[winfo class $w] eq \"Menu\"} {\n\
tk_menuSetFocus $w\n\
set Priv(window) $w\n\
SaveGrabInfo $w\n\
grab -global $w\n\
MenuFirstEntry $w\n\
} else {\n\
MbPost $w\n\
MenuFirstEntry [$w cget -menu]\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::TraverseWithinMenu {w char} {\n\
if {$char eq \"\"} {\n\
return\n\
}\n\
set char [string tolower $char]\n\
set last [$w index last]\n\
if {$last eq \"none\"} {\n\
return\n\
}\n\
for {set i 0} {$i <= $last} {incr i} {\n\
if {[catch {set char2 [string index \\\n\
[$w entrycget $i -label] [$w entrycget $i -underline]]}]} {\n\
continue\n\
}\n\
if {$char eq [string tolower $char2]} {\n\
if {[$w type $i] eq \"cascade\"} {\n\
$w activate $i\n\
$w postcascade active\n\
event generate $w <<MenuSelect>>\n\
set m2 [$w entrycget $i -menu]\n\
if {$m2 ne \"\"} {\n\
MenuFirstEntry $m2\n\
}\n\
} else {\n\
MenuUnpost $w\n\
uplevel #0 [list $w invoke $i]\n\
}\n\
return\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MenuFirstEntry menu {\n\
if {$menu eq \"\"} {\n\
return\n\
}\n\
tk_menuSetFocus $menu\n\
if {[$menu index active] ne \"none\"} {\n\
return\n\
}\n\
set last [$menu index last]\n\
if {$last eq \"none\"} {\n\
return\n\
}\n\
for {set i 0} {$i <= $last} {incr i} {\n\
if {([catch {set state [$menu entrycget $i -state]}] == 0) \\\n\
&& $state ne \"disabled\" && [$menu type $i] ne \"tearoff\"} {\n\
$menu activate $i\n\
GenerateMenuSelect $menu\n\
if {[$menu type $i] eq \"cascade\" \\\n\
&& [$menu cget -type] eq \"menubar\"} {\n\
set cascade [$menu entrycget $i -menu]\n\
if {$cascade ne \"\"} {\n\
$menu postcascade $i\n\
MenuFirstEntry $cascade\n\
}\n\
}\n\
return\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::MenuFindName {menu s} {\n\
set i \"\"\n\
if {![regexp {^active$|^last$|^none$|^[0-9]|^@} $s]} {\n\
catch {set i [$menu index $s]}\n\
return $i\n\
}\n\
set last [$menu index last]\n\
if {$last eq \"none\"} {\n\
return\n\
}\n\
for {set i 0} {$i <= $last} {incr i} {\n\
if {![catch {$menu entrycget $i -label} label]} {\n\
if {$label eq $s} {\n\
return $i\n\
}\n\
}\n\
}\n\
return \"\"\n\
}\n\
\n\
\n\
proc ::tk::PostOverPoint {menu x y {entry {}}}  {\n\
global tcl_platform\n\
\n\
if {$entry ne \"\"} {\n\
if {$entry == [$menu index last]} {\n\
incr y [expr {-([$menu yposition $entry] \\\n\
+ [winfo reqheight $menu])/2}]\n\
} else {\n\
incr y [expr {-([$menu yposition $entry] \\\n\
+ [$menu yposition [expr {$entry+1}]])/2}]\n\
}\n\
incr x [expr {-[winfo reqwidth $menu]/2}]\n\
}\n\
\n\
if {$tcl_platform(platform) eq \"windows\"} {\n\
set ver 5\n\
if {[info exists tcl_platform(osVersion)]} {\n\
scan $tcl_platform(osVersion) %d ver\n\
}\n\
\n\
if {$ver < 6} {\n\
set yoffset [expr {[winfo screenheight $menu] \\\n\
- $y - [winfo reqheight $menu] - 10}]\n\
if {$yoffset < 0} {\n\
incr y $yoffset\n\
if {$y < 0} { set y 0 }\n\
}\n\
if {$y < 0} {\n\
set y 0\n\
}\n\
}\n\
}\n\
$menu post $x $y\n\
if {$entry ne \"\" && [$menu entrycget $entry -state] ne \"disabled\"} {\n\
$menu activate $entry\n\
GenerateMenuSelect $menu\n\
}\n\
}\n\
\n\
\n\
proc tk::SaveGrabInfo w {\n\
variable ::tk::Priv\n\
set Priv(oldGrab) [grab current $w]\n\
if {$Priv(oldGrab) ne \"\"} {\n\
set Priv(grabStatus) [grab status $Priv(oldGrab)]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::RestoreOldGrab {} {\n\
variable ::tk::Priv\n\
\n\
if {$Priv(oldGrab) ne \"\"} {\n\
\n\
catch {\n\
if {$Priv(grabStatus) eq \"global\"} {\n\
grab set -global $Priv(oldGrab)\n\
} else {\n\
grab set $Priv(oldGrab)\n\
}\n\
}\n\
set Priv(oldGrab) \"\"\n\
}\n\
}\n\
\n\
proc ::tk_menuSetFocus {menu} {\n\
variable ::tk::Priv\n\
if {![info exists Priv(focus)] || $Priv(focus) eq \"\"} {\n\
set Priv(focus) [focus]\n\
}\n\
focus $menu\n\
}\n\
\n\
proc ::tk::GenerateMenuSelect {menu} {\n\
variable ::tk::Priv\n\
\n\
if {$Priv(activeMenu) eq $menu \\\n\
&& $Priv(activeItem) eq [$menu index active]} {\n\
return\n\
}\n\
\n\
set Priv(activeMenu) $menu\n\
set Priv(activeItem) [$menu index active]\n\
event generate $menu <<MenuSelect>>\n\
}\n\
\n\
\n\
proc ::tk_popup {menu x y {entry {}}} {\n\
variable ::tk::Priv\n\
global tcl_platform\n\
if {$Priv(popup) ne \"\" || $Priv(postedMb) ne \"\"} {\n\
tk::MenuUnpost {}\n\
}\n\
tk::PostOverPoint $menu $x $y $entry\n\
if {[tk windowingsystem] eq \"x11\" && [winfo viewable $menu]} {\n\
tk::SaveGrabInfo $menu\n\
grab -global $menu\n\
set Priv(popup) $menu\n\
set Priv(menuActivated) 1\n\
tk_menuSetFocus $menu\n\
}\n\
}\n\
\n\
\n\
proc ::tk_setPalette {args} {\n\
if {[winfo depth .] == 1} {\n\
return\n\
}\n\
\n\
\n\
if {[llength $args] == 1} {\n\
set new(background) [lindex $args 0]\n\
} else {\n\
array set new $args\n\
}\n\
if {![info exists new(background)]} {\n\
error \"must specify a background color\"\n\
}\n\
set bg [winfo rgb . $new(background)]\n\
if {![info exists new(foreground)]} {\n\
foreach {r g b} $bg {break}\n\
if {$r+1.5*$g+0.5*$b > 100000} {\n\
set new(foreground) black\n\
} else {\n\
set new(foreground) white\n\
}\n\
}\n\
lassign [winfo rgb . $new(foreground)] fg_r fg_g fg_b\n\
lassign $bg bg_r bg_g bg_b\n\
set darkerBg [format #%02x%02x%02x [expr {(9*$bg_r)/2560}] \\\n\
[expr {(9*$bg_g)/2560}] [expr {(9*$bg_b)/2560}]]\n\
\n\
foreach i {activeForeground insertBackground selectForeground \\\n\
highlightColor} {\n\
if {![info exists new($i)]} {\n\
set new($i) $new(foreground)\n\
}\n\
}\n\
if {![info exists new(disabledForeground)]} {\n\
set new(disabledForeground) [format #%02x%02x%02x \\\n\
[expr {(3*$bg_r + $fg_r)/1024}] \\\n\
[expr {(3*$bg_g + $fg_g)/1024}] \\\n\
[expr {(3*$bg_b + $fg_b)/1024}]]\n\
}\n\
if {![info exists new(highlightBackground)]} {\n\
set new(highlightBackground) $new(background)\n\
}\n\
if {![info exists new(activeBackground)]} {\n\
\n\
foreach i {0 1 2} color $bg {\n\
set light($i) [expr {$color/256}]\n\
set inc1 [expr {($light($i)*15)/100}]\n\
set inc2 [expr {(255-$light($i))/3}]\n\
if {$inc1 > $inc2} {\n\
incr light($i) $inc1\n\
} else {\n\
incr light($i) $inc2\n\
}\n\
if {$light($i) > 255} {\n\
set light($i) 255\n\
}\n\
}\n\
set new(activeBackground) [format #%02x%02x%02x $light(0) \\\n\
$light(1) $light(2)]\n\
}\n\
if {![info exists new(selectBackground)]} {\n\
set new(selectBackground) $darkerBg\n\
}\n\
if {![info exists new(troughColor)]} {\n\
set new(troughColor) $darkerBg\n\
}\n\
\n\
toplevel .___tk_set_palette\n\
wm withdraw .___tk_set_palette\n\
foreach q {\n\
button canvas checkbutton entry frame label labelframe\n\
listbox menubutton menu message radiobutton scale scrollbar\n\
spinbox text\n\
} {\n\
$q .___tk_set_palette.$q\n\
}\n\
\n\
\n\
eval [tk::RecolorTree . new]\n\
\n\
destroy .___tk_set_palette\n\
\n\
\n\
foreach option [array names new] {\n\
option add *$option $new($option) widgetDefault\n\
}\n\
\n\
\n\
array set ::tk::Palette [array get new]\n\
}\n\
\n\
\n\
proc ::tk::RecolorTree {w colors} {\n\
upvar $colors c\n\
set result {}\n\
set prototype .___tk_set_palette.[string tolower [winfo class $w]]\n\
if {![winfo exists $prototype]} {\n\
unset prototype\n\
}\n\
foreach dbOption [array names c] {\n\
set option -[string tolower $dbOption]\n\
set class [string replace $dbOption 0 0 [string toupper \\\n\
[string index $dbOption 0]]]\n\
if {![catch {$w configure $option} value]} {\n\
set defaultcolor [option get $w $dbOption $class]\n\
if {$defaultcolor eq \"\" || \\\n\
([info exists prototype] && \\\n\
[$prototype cget $option] ne \"$defaultcolor\")} {\n\
set defaultcolor [lindex $value 3]\n\
}\n\
if {$defaultcolor ne \"\"} {\n\
set defaultcolor [winfo rgb . $defaultcolor]\n\
}\n\
set chosencolor [lindex $value 4]\n\
if {$chosencolor ne \"\"} {\n\
set chosencolor [winfo rgb . $chosencolor]\n\
}\n\
if {[string match $defaultcolor $chosencolor]} {\n\
append result \";\\noption add [list \\\n\
*[winfo class $w].$dbOption $c($dbOption) 60]\"\n\
$w configure $option $c($dbOption)\n\
}\n\
}\n\
}\n\
foreach child [winfo children $w] {\n\
append result \";\\n[::tk::RecolorTree $child c]\"\n\
}\n\
return $result\n\
}\n\
\n\
\n\
proc ::tk::Darken {color percent} {\n\
foreach {red green blue} [winfo rgb . $color] {\n\
set red [expr {($red/256)*$percent/100}]\n\
set green [expr {($green/256)*$percent/100}]\n\
set blue [expr {($blue/256)*$percent/100}]\n\
break\n\
}\n\
if {$red > 255} {\n\
set red 255\n\
}\n\
if {$green > 255} {\n\
set green 255\n\
}\n\
if {$blue > 255} {\n\
set blue 255\n\
}\n\
return [format \"#%02x%02x%02x\" $red $green $blue]\n\
}\n\
\n\
\n\
proc ::tk_bisque {} {\n\
tk_setPalette activeBackground #e6ceb1 activeForeground black \\\n\
background #ffe4c4 disabledForeground #b0b0b0 foreground black \\\n\
highlightBackground #ffe4c4 highlightColor black \\\n\
insertBackground black \\\n\
selectBackground #e6ceb1 selectForeground black \\\n\
troughColor #cdb79e\n\
}\n\
\n\
\n\
\n\
bind Scale <Enter> {\n\
if {$tk_strictMotif} {\n\
set tk::Priv(activeBg) [%W cget -activebackground]\n\
%W configure -activebackground [%W cget -background]\n\
}\n\
tk::ScaleActivate %W %x %y\n\
}\n\
bind Scale <Motion> {\n\
tk::ScaleActivate %W %x %y\n\
}\n\
bind Scale <Leave> {\n\
if {$tk_strictMotif} {\n\
%W configure -activebackground $tk::Priv(activeBg)\n\
}\n\
if {[%W cget -state] eq \"active\"} {\n\
%W configure -state normal\n\
}\n\
}\n\
bind Scale <1> {\n\
tk::ScaleButtonDown %W %x %y\n\
}\n\
bind Scale <B1-Motion> {\n\
tk::ScaleDrag %W %x %y\n\
}\n\
bind Scale <B1-Leave> { }\n\
bind Scale <B1-Enter> { }\n\
bind Scale <ButtonRelease-1> {\n\
tk::CancelRepeat\n\
tk::ScaleEndDrag %W\n\
tk::ScaleActivate %W %x %y\n\
}\n\
bind Scale <2> {\n\
tk::ScaleButton2Down %W %x %y\n\
}\n\
bind Scale <B2-Motion> {\n\
tk::ScaleDrag %W %x %y\n\
}\n\
bind Scale <B2-Leave> { }\n\
bind Scale <B2-Enter> { }\n\
bind Scale <ButtonRelease-2> {\n\
tk::CancelRepeat\n\
tk::ScaleEndDrag %W\n\
tk::ScaleActivate %W %x %y\n\
}\n\
if {$tcl_platform(platform) eq \"windows\"} {\n\
bind Scale <3>		[bind Scale <2>]\n\
bind Scale <B3-Motion>	[bind Scale <B2-Motion>]\n\
bind Scale <B3-Leave>	[bind Scale <B2-Leave>]\n\
bind Scale <B3-Enter>	[bind Scale <B2-Enter>]\n\
bind Scale <ButtonRelease-3> [bind Scale <ButtonRelease-2>]\n\
}\n\
bind Scale <Control-1> {\n\
tk::ScaleControlPress %W %x %y\n\
}\n\
bind Scale <Up> {\n\
tk::ScaleIncrement %W up little noRepeat\n\
}\n\
bind Scale <Down> {\n\
tk::ScaleIncrement %W down little noRepeat\n\
}\n\
bind Scale <Left> {\n\
tk::ScaleIncrement %W up little noRepeat\n\
}\n\
bind Scale <Right> {\n\
tk::ScaleIncrement %W down little noRepeat\n\
}\n\
bind Scale <Control-Up> {\n\
tk::ScaleIncrement %W up big noRepeat\n\
}\n\
bind Scale <Control-Down> {\n\
tk::ScaleIncrement %W down big noRepeat\n\
}\n\
bind Scale <Control-Left> {\n\
tk::ScaleIncrement %W up big noRepeat\n\
}\n\
bind Scale <Control-Right> {\n\
tk::ScaleIncrement %W down big noRepeat\n\
}\n\
bind Scale <Home> {\n\
%W set [%W cget -from]\n\
}\n\
bind Scale <End> {\n\
%W set [%W cget -to]\n\
}\n\
\n\
\n\
proc ::tk::ScaleActivate {w x y} {\n\
if {[$w cget -state] eq \"disabled\"} {\n\
return\n\
}\n\
if {[$w identify $x $y] eq \"slider\"} {\n\
set state active\n\
} else {\n\
set state normal\n\
}\n\
if {[$w cget -state] ne $state} {\n\
$w configure -state $state\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScaleButtonDown {w x y} {\n\
variable ::tk::Priv\n\
set Priv(dragging) 0\n\
set el [$w identify $x $y]\n\
\n\
set Priv($w,relief) [$w cget -sliderrelief]\n\
\n\
if {$el eq \"trough1\"} {\n\
ScaleIncrement $w up little initial\n\
} elseif {$el eq \"trough2\"} {\n\
ScaleIncrement $w down little initial\n\
} elseif {$el eq \"slider\"} {\n\
set Priv(dragging) 1\n\
set Priv(initValue) [$w get]\n\
set coords [$w coords]\n\
set Priv(deltaX) [expr {$x - [lindex $coords 0]}]\n\
set Priv(deltaY) [expr {$y - [lindex $coords 1]}]\n\
switch -exact -- $Priv($w,relief) {\n\
\"raised\" { $w configure -sliderrelief sunken }\n\
\"ridge\"  { $w configure -sliderrelief groove }\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScaleDrag {w x y} {\n\
variable ::tk::Priv\n\
if {!$Priv(dragging)} {\n\
return\n\
}\n\
$w set [$w get [expr {$x-$Priv(deltaX)}] [expr {$y-$Priv(deltaY)}]]\n\
}\n\
\n\
\n\
proc ::tk::ScaleEndDrag {w} {\n\
variable ::tk::Priv\n\
set Priv(dragging) 0\n\
if {[info exists Priv($w,relief)]} {\n\
$w configure -sliderrelief $Priv($w,relief)\n\
unset Priv($w,relief)\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScaleIncrement {w dir big repeat} {\n\
variable ::tk::Priv\n\
if {![winfo exists $w]} return\n\
if {$big eq \"big\"} {\n\
set inc [$w cget -bigincrement]\n\
if {$inc == 0} {\n\
set inc [expr {abs([$w cget -to] - [$w cget -from])/10.0}]\n\
}\n\
if {$inc < [$w cget -resolution]} {\n\
set inc [$w cget -resolution]\n\
}\n\
} else {\n\
set inc [$w cget -resolution]\n\
}\n\
if {([$w cget -from] > [$w cget -to]) ^ ($dir eq \"up\")} {\n\
set inc [expr {-$inc}]\n\
}\n\
$w set [expr {[$w get] + $inc}]\n\
\n\
if {$repeat eq \"again\"} {\n\
set Priv(afterId) [after [$w cget -repeatinterval] \\\n\
[list tk::ScaleIncrement $w $dir $big again]]\n\
} elseif {$repeat eq \"initial\"} {\n\
set delay [$w cget -repeatdelay]\n\
if {$delay > 0} {\n\
set Priv(afterId) [after $delay \\\n\
[list tk::ScaleIncrement $w $dir $big again]]\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScaleControlPress {w x y} {\n\
set el [$w identify $x $y]\n\
if {$el eq \"trough1\"} {\n\
$w set [$w cget -from]\n\
} elseif {$el eq \"trough2\"} {\n\
$w set [$w cget -to]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScaleButton2Down {w x y} {\n\
variable ::tk::Priv\n\
\n\
if {[$w cget -state] eq \"disabled\"} {\n\
return\n\
}\n\
\n\
$w configure -state active\n\
$w set [$w get $x $y]\n\
set Priv(dragging) 1\n\
set Priv(initValue) [$w get]\n\
set Priv($w,relief) [$w cget -sliderrelief]\n\
set coords \"$x $y\"\n\
set Priv(deltaX) 0\n\
set Priv(deltaY) 0\n\
}\n\
\n\
\n\
proc ::tk::TearOffMenu {w {x 0} {y 0}} {\n\
\n\
if {$x == 0} {\n\
set x [winfo rootx $w]\n\
}\n\
if {$y == 0} {\n\
set y [winfo rooty $w]\n\
if {[tk windowingsystem] eq \"aqua\"} {\n\
catch {incr y [expr {[$w yposition 1] - 16}]}\n\
if {$y < 22} { set y 22 }\n\
}\n\
}\n\
\n\
set parent [winfo parent $w]\n\
while {[winfo toplevel $parent] ne $parent \\\n\
|| [winfo class $parent] eq \"Menu\"} {\n\
set parent [winfo parent $parent]\n\
}\n\
if {$parent eq \".\"} {\n\
set parent \"\"\n\
}\n\
for {set i 1} 1 {incr i} {\n\
set menu $parent.tearoff$i\n\
if {![winfo exists $menu]} {\n\
break\n\
}\n\
}\n\
\n\
$w clone $menu tearoff\n\
\n\
\n\
set parent [winfo parent $w]\n\
if {[$menu cget -title] ne \"\"} {\n\
wm title $menu [$menu cget -title]\n\
} else {\n\
switch -- [winfo class $parent] {\n\
Menubutton {\n\
wm title $menu [$parent cget -text]\n\
}\n\
Menu {\n\
wm title $menu [$parent entrycget active -label]\n\
}\n\
}\n\
}\n\
\n\
if {[tk windowingsystem] eq \"win32\"} {\n\
set parent [winfo toplevel $parent]\n\
while {[winfo class $parent] eq \"Menu\"} {\n\
set parent [winfo toplevel [winfo parent $parent]]\n\
}\n\
wm transient $menu [winfo toplevel $parent]\n\
wm attributes $menu -toolwindow 1\n\
}\n\
\n\
$menu post $x $y\n\
\n\
if {[winfo exists $menu] == 0} {\n\
return \"\"\n\
}\n\
\n\
\n\
bind $menu <Enter> {\n\
set tk::Priv(focus) %W\n\
}\n\
\n\
\n\
set cmd [$w cget -tearoffcommand]\n\
if {$cmd ne \"\"} {\n\
uplevel #0 $cmd [list $w $menu]\n\
}\n\
return $menu\n\
}\n\
\n\
\n\
proc ::tk::MenuDup {src dst type} {\n\
set cmd [list menu $dst -type $type]\n\
foreach option [$src configure] {\n\
if {[llength $option] == 2} {\n\
continue\n\
}\n\
if {[lindex $option 0] eq \"-type\"} {\n\
continue\n\
}\n\
lappend cmd [lindex $option 0] [lindex $option 4]\n\
}\n\
eval $cmd\n\
set last [$src index last]\n\
if {$last eq \"none\"} {\n\
return\n\
}\n\
for {set i [$src cget -tearoff]} {$i <= $last} {incr i} {\n\
set cmd [list $dst add [$src type $i]]\n\
foreach option [$src entryconfigure $i]  {\n\
lappend cmd [lindex $option 0] [lindex $option 4]\n\
}\n\
eval $cmd\n\
}\n\
\n\
\n\
set tags [bindtags $src]\n\
set srcLen [string length $src]\n\
\n\
\n\
while {[set index [string first $src $tags]] != -1} {\n\
append x [string range $tags 0 [expr {$index - 1}]]$dst\n\
set tags [string range $tags [expr {$index + $srcLen}] end]\n\
}\n\
append x $tags\n\
\n\
bindtags $dst $x\n\
\n\
foreach event [bind $src] {\n\
unset x\n\
set script [bind $src $event]\n\
set eventLen [string length $event]\n\
\n\
\n\
while {[set index [string first $event $script]] != -1} {\n\
append x [string range $script 0 [expr {$index - 1}]]\n\
append x $dst\n\
set script [string range $script [expr {$index + $eventLen}] end]\n\
}\n\
append x $script\n\
\n\
bind $dst $event $x\n\
}\n\
}\n\
\n\
\n\
\n\
\n\
bind Text <1> {\n\
tk::TextButton1 %W %x %y\n\
%W tag remove sel 0.0 end\n\
}\n\
bind Text <B1-Motion> {\n\
set tk::Priv(x) %x\n\
set tk::Priv(y) %y\n\
tk::TextSelectTo %W %x %y\n\
}\n\
bind Text <Double-1> {\n\
set tk::Priv(selectMode) word\n\
tk::TextSelectTo %W %x %y\n\
catch {%W mark set insert sel.first}\n\
}\n\
bind Text <Triple-1> {\n\
set tk::Priv(selectMode) line\n\
tk::TextSelectTo %W %x %y\n\
catch {%W mark set insert sel.first}\n\
}\n\
bind Text <Shift-1> {\n\
tk::TextResetAnchor %W @%x,%y\n\
set tk::Priv(selectMode) char\n\
tk::TextSelectTo %W %x %y\n\
}\n\
bind Text <Double-Shift-1>	{\n\
set tk::Priv(selectMode) word\n\
tk::TextSelectTo %W %x %y 1\n\
}\n\
bind Text <Triple-Shift-1>	{\n\
set tk::Priv(selectMode) line\n\
tk::TextSelectTo %W %x %y\n\
}\n\
bind Text <B1-Leave> {\n\
set tk::Priv(x) %x\n\
set tk::Priv(y) %y\n\
tk::TextAutoScan %W\n\
}\n\
bind Text <B1-Enter> {\n\
tk::CancelRepeat\n\
}\n\
bind Text <ButtonRelease-1> {\n\
tk::CancelRepeat\n\
}\n\
bind Text <Control-1> {\n\
%W mark set insert @%x,%y\n\
}\n\
bind Text <Left> {\n\
tk::TextSetCursor %W insert-1displayindices\n\
}\n\
bind Text <Right> {\n\
tk::TextSetCursor %W insert+1displayindices\n\
}\n\
bind Text <Up> {\n\
tk::TextSetCursor %W [tk::TextUpDownLine %W -1]\n\
}\n\
bind Text <Down> {\n\
tk::TextSetCursor %W [tk::TextUpDownLine %W 1]\n\
}\n\
bind Text <Shift-Left> {\n\
tk::TextKeySelect %W [%W index {insert - 1displayindices}]\n\
}\n\
bind Text <Shift-Right> {\n\
tk::TextKeySelect %W [%W index {insert + 1displayindices}]\n\
}\n\
bind Text <Shift-Up> {\n\
tk::TextKeySelect %W [tk::TextUpDownLine %W -1]\n\
}\n\
bind Text <Shift-Down> {\n\
tk::TextKeySelect %W [tk::TextUpDownLine %W 1]\n\
}\n\
bind Text <Control-Left> {\n\
tk::TextSetCursor %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]\n\
}\n\
bind Text <Control-Right> {\n\
tk::TextSetCursor %W [tk::TextNextWord %W insert]\n\
}\n\
bind Text <Control-Up> {\n\
tk::TextSetCursor %W [tk::TextPrevPara %W insert]\n\
}\n\
bind Text <Control-Down> {\n\
tk::TextSetCursor %W [tk::TextNextPara %W insert]\n\
}\n\
bind Text <Shift-Control-Left> {\n\
tk::TextKeySelect %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]\n\
}\n\
bind Text <Shift-Control-Right> {\n\
tk::TextKeySelect %W [tk::TextNextWord %W insert]\n\
}\n\
bind Text <Shift-Control-Up> {\n\
tk::TextKeySelect %W [tk::TextPrevPara %W insert]\n\
}\n\
bind Text <Shift-Control-Down> {\n\
tk::TextKeySelect %W [tk::TextNextPara %W insert]\n\
}\n\
bind Text <Prior> {\n\
tk::TextSetCursor %W [tk::TextScrollPages %W -1]\n\
}\n\
bind Text <Shift-Prior> {\n\
tk::TextKeySelect %W [tk::TextScrollPages %W -1]\n\
}\n\
bind Text <Next> {\n\
tk::TextSetCursor %W [tk::TextScrollPages %W 1]\n\
}\n\
bind Text <Shift-Next> {\n\
tk::TextKeySelect %W [tk::TextScrollPages %W 1]\n\
}\n\
bind Text <Control-Prior> {\n\
%W xview scroll -1 page\n\
}\n\
bind Text <Control-Next> {\n\
%W xview scroll 1 page\n\
}\n\
\n\
bind Text <Home> {\n\
tk::TextSetCursor %W {insert display linestart}\n\
}\n\
bind Text <Shift-Home> {\n\
tk::TextKeySelect %W {insert display linestart}\n\
}\n\
bind Text <End> {\n\
tk::TextSetCursor %W {insert display lineend}\n\
}\n\
bind Text <Shift-End> {\n\
tk::TextKeySelect %W {insert display lineend}\n\
}\n\
bind Text <Control-Home> {\n\
tk::TextSetCursor %W 1.0\n\
}\n\
bind Text <Control-Shift-Home> {\n\
tk::TextKeySelect %W 1.0\n\
}\n\
bind Text <Control-End> {\n\
tk::TextSetCursor %W {end - 1 indices}\n\
}\n\
bind Text <Control-Shift-End> {\n\
tk::TextKeySelect %W {end - 1 indices}\n\
}\n\
\n\
bind Text <Tab> {\n\
if {[%W cget -state] eq \"normal\"} {\n\
tk::TextInsert %W \\t\n\
focus %W\n\
break\n\
}\n\
}\n\
bind Text <Shift-Tab> {\n\
break\n\
}\n\
bind Text <Control-Tab> {\n\
focus [tk_focusNext %W]\n\
}\n\
bind Text <Control-Shift-Tab> {\n\
focus [tk_focusPrev %W]\n\
}\n\
bind Text <Control-i> {\n\
tk::TextInsert %W \\t\n\
}\n\
bind Text <Return> {\n\
tk::TextInsert %W \\n\n\
if {[%W cget -autoseparators]} {\n\
%W edit separator\n\
}\n\
}\n\
bind Text <Delete> {\n\
if {[tk::TextCursorInSelection %W]} {\n\
%W delete sel.first sel.last\n\
} elseif {[%W compare end != insert+1c]} {\n\
%W delete insert\n\
}\n\
%W see insert\n\
}\n\
bind Text <BackSpace> {\n\
if {[tk::TextCursorInSelection %W]} {\n\
%W delete sel.first sel.last\n\
} elseif {[%W compare insert != 1.0]} {\n\
%W delete insert-1c\n\
}\n\
%W see insert\n\
}\n\
\n\
bind Text <Control-space> {\n\
%W mark set [tk::TextAnchor %W] insert\n\
}\n\
bind Text <Select> {\n\
%W mark set [tk::TextAnchor %W] insert\n\
}\n\
bind Text <Control-Shift-space> {\n\
set tk::Priv(selectMode) char\n\
tk::TextKeyExtend %W insert\n\
}\n\
bind Text <Shift-Select> {\n\
set tk::Priv(selectMode) char\n\
tk::TextKeyExtend %W insert\n\
}\n\
bind Text <Control-slash> {\n\
%W tag add sel 1.0 end\n\
}\n\
bind Text <Control-backslash> {\n\
%W tag remove sel 1.0 end\n\
}\n\
bind Text <<Cut>> {\n\
tk_textCut %W\n\
}\n\
bind Text <<Copy>> {\n\
tk_textCopy %W\n\
}\n\
bind Text <<Paste>> {\n\
tk_textPaste %W\n\
}\n\
bind Text <<Clear>> {\n\
catch {%W delete sel.first sel.last}\n\
}\n\
bind Text <<PasteSelection>> {\n\
if {$tk_strictMotif || ![info exists tk::Priv(mouseMoved)]\n\
|| !$tk::Priv(mouseMoved)} {\n\
tk::TextPasteSelection %W %x %y\n\
}\n\
}\n\
bind Text <Insert> {\n\
catch {tk::TextInsert %W [::tk::GetSelection %W PRIMARY]}\n\
}\n\
bind Text <KeyPress> {\n\
tk::TextInsert %W %A\n\
}\n\
\n\
\n\
bind Text <Alt-KeyPress> {# nothing }\n\
bind Text <Meta-KeyPress> {# nothing}\n\
bind Text <Control-KeyPress> {# nothing}\n\
bind Text <Escape> {# nothing}\n\
bind Text <KP_Enter> {# nothing}\n\
if {[tk windowingsystem] eq \"aqua\"} {\n\
bind Text <Command-KeyPress> {# nothing}\n\
}\n\
\n\
\n\
bind Text <Control-a> {\n\
if {!$tk_strictMotif} {\n\
tk::TextSetCursor %W {insert display linestart}\n\
}\n\
}\n\
bind Text <Control-b> {\n\
if {!$tk_strictMotif} {\n\
tk::TextSetCursor %W insert-1displayindices\n\
}\n\
}\n\
bind Text <Control-d> {\n\
if {!$tk_strictMotif && [%W compare end != insert+1c]} {\n\
%W delete insert\n\
}\n\
}\n\
bind Text <Control-e> {\n\
if {!$tk_strictMotif} {\n\
tk::TextSetCursor %W {insert display lineend}\n\
}\n\
}\n\
bind Text <Control-f> {\n\
if {!$tk_strictMotif} {\n\
tk::TextSetCursor %W insert+1displayindices\n\
}\n\
}\n\
bind Text <Control-k> {\n\
if {!$tk_strictMotif && [%W compare end != insert+1c]} {\n\
if {[%W compare insert == {insert lineend}]} {\n\
%W delete insert\n\
} else {\n\
%W delete insert {insert lineend}\n\
}\n\
}\n\
}\n\
bind Text <Control-n> {\n\
if {!$tk_strictMotif} {\n\
tk::TextSetCursor %W [tk::TextUpDownLine %W 1]\n\
}\n\
}\n\
bind Text <Control-o> {\n\
if {!$tk_strictMotif} {\n\
%W insert insert \\n\n\
%W mark set insert insert-1c\n\
}\n\
}\n\
bind Text <Control-p> {\n\
if {!$tk_strictMotif} {\n\
tk::TextSetCursor %W [tk::TextUpDownLine %W -1]\n\
}\n\
}\n\
bind Text <Control-t> {\n\
if {!$tk_strictMotif} {\n\
tk::TextTranspose %W\n\
}\n\
}\n\
\n\
bind Text <<Undo>> {\n\
catch { %W edit undo }\n\
}\n\
\n\
bind Text <<Redo>> {\n\
catch { %W edit redo }\n\
}\n\
\n\
bind Text <Meta-b> {\n\
if {!$tk_strictMotif} {\n\
tk::TextSetCursor %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]\n\
}\n\
}\n\
bind Text <Meta-d> {\n\
if {!$tk_strictMotif && [%W compare end != insert+1c]} {\n\
%W delete insert [tk::TextNextWord %W insert]\n\
}\n\
}\n\
bind Text <Meta-f> {\n\
if {!$tk_strictMotif} {\n\
tk::TextSetCursor %W [tk::TextNextWord %W insert]\n\
}\n\
}\n\
bind Text <Meta-less> {\n\
if {!$tk_strictMotif} {\n\
tk::TextSetCursor %W 1.0\n\
}\n\
}\n\
bind Text <Meta-greater> {\n\
if {!$tk_strictMotif} {\n\
tk::TextSetCursor %W end-1c\n\
}\n\
}\n\
bind Text <Meta-BackSpace> {\n\
if {!$tk_strictMotif} {\n\
%W delete [tk::TextPrevPos %W insert tcl_startOfPreviousWord] insert\n\
}\n\
}\n\
bind Text <Meta-Delete> {\n\
if {!$tk_strictMotif} {\n\
%W delete [tk::TextPrevPos %W insert tcl_startOfPreviousWord] insert\n\
}\n\
}\n\
\n\
\n\
if {[tk windowingsystem] eq \"aqua\"} {\n\
bind Text <Option-Left> {\n\
tk::TextSetCursor %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]\n\
}\n\
bind Text <Option-Right> {\n\
tk::TextSetCursor %W [tk::TextNextWord %W insert]\n\
}\n\
bind Text <Option-Up> {\n\
tk::TextSetCursor %W [tk::TextPrevPara %W insert]\n\
}\n\
bind Text <Option-Down> {\n\
tk::TextSetCursor %W [tk::TextNextPara %W insert]\n\
}\n\
bind Text <Shift-Option-Left> {\n\
tk::TextKeySelect %W [tk::TextPrevPos %W insert tcl_startOfPreviousWord]\n\
}\n\
bind Text <Shift-Option-Right> {\n\
tk::TextKeySelect %W [tk::TextNextWord %W insert]\n\
}\n\
bind Text <Shift-Option-Up> {\n\
tk::TextKeySelect %W [tk::TextPrevPara %W insert]\n\
}\n\
bind Text <Shift-Option-Down> {\n\
tk::TextKeySelect %W [tk::TextNextPara %W insert]\n\
}\n\
bind Text <Control-v> {\n\
tk::TextScrollPages %W 1\n\
}\n\
\n\
}\n\
\n\
\n\
bind Text <Control-h> {\n\
if {!$tk_strictMotif && [%W compare insert != 1.0]} {\n\
%W delete insert-1c\n\
%W see insert\n\
}\n\
}\n\
bind Text <2> {\n\
if {!$tk_strictMotif} {\n\
tk::TextScanMark %W %x %y\n\
}\n\
}\n\
bind Text <B2-Motion> {\n\
if {!$tk_strictMotif} {\n\
tk::TextScanDrag %W %x %y\n\
}\n\
}\n\
set ::tk::Priv(prevPos) {}\n\
\n\
\n\
if {[tk windowingsystem] eq \"aqua\"} {\n\
bind Text <MouseWheel> {\n\
%W yview scroll [expr {-15 * (%D)}] pixels\n\
}\n\
bind Text <Option-MouseWheel> {\n\
%W yview scroll [expr {-150 * (%D)}] pixels\n\
}\n\
bind Text <Shift-MouseWheel> {\n\
%W xview scroll [expr {-15 * (%D)}] pixels\n\
}\n\
bind Text <Shift-Option-MouseWheel> {\n\
%W xview scroll [expr {-150 * (%D)}] pixels\n\
}\n\
} else {\n\
bind Text <MouseWheel> {\n\
if {%D >= 0} {\n\
%W yview scroll [expr {-%D/3}] pixels\n\
} else {\n\
%W yview scroll [expr {(2-%D)/3}] pixels\n\
}\n\
}\n\
}\n\
\n\
if {\"x11\" eq [tk windowingsystem]} {\n\
bind Text <4> {\n\
if {!$tk_strictMotif} {\n\
%W yview scroll -50 pixels\n\
}\n\
}\n\
bind Text <5> {\n\
if {!$tk_strictMotif} {\n\
%W yview scroll 50 pixels\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::TextClosestGap {w x y} {\n\
set pos [$w index @$x,$y]\n\
set bbox [$w bbox $pos]\n\
if {$bbox eq \"\"} {\n\
return $pos\n\
}\n\
if {($x - [lindex $bbox 0]) < ([lindex $bbox 2]/2)} {\n\
return $pos\n\
}\n\
$w index \"$pos + 1 char\"\n\
}\n\
\n\
\n\
proc ::tk::TextButton1 {w x y} {\n\
variable ::tk::Priv\n\
\n\
set Priv(selectMode) char\n\
set Priv(mouseMoved) 0\n\
set Priv(pressX) $x\n\
set anchorname [tk::TextAnchor $w]\n\
$w mark set insert [TextClosestGap $w $x $y]\n\
$w mark set $anchorname insert\n\
set bbox [$w bbox [$w index $anchorname]]\n\
if {$x > [lindex $bbox 0]} {\n\
$w mark gravity $anchorname right\n\
} else {\n\
$w mark gravity $anchorname left\n\
}\n\
if {$::tcl_platform(platform) eq \"windows\" \\\n\
|| [$w cget -state] eq \"normal\"} {\n\
focus $w\n\
}\n\
if {[$w cget -autoseparators]} {\n\
$w edit separator\n\
}\n\
}\n\
\n\
\n\
set ::tk::Priv(textanchoruid) 0\n\
\n\
proc ::tk::TextAnchor {w} {\n\
variable Priv\n\
if {![info exists Priv(textanchor,$w)]} {\n\
set Priv(textanchor,$w) tk::anchor[incr Priv(textanchoruid)]\n\
}\n\
return $Priv(textanchor,$w)\n\
}\n\
\n\
proc ::tk::TextSelectTo {w x y {extend 0}} {\n\
global tcl_platform\n\
variable ::tk::Priv\n\
\n\
set anchorname [tk::TextAnchor $w]\n\
set cur [TextClosestGap $w $x $y]\n\
if {[catch {$w index $anchorname}]} {\n\
$w mark set $anchorname $cur\n\
}\n\
set anchor [$w index $anchorname]\n\
if {[$w compare $cur != $anchor] || (abs($Priv(pressX) - $x) >= 3)} {\n\
set Priv(mouseMoved) 1\n\
}\n\
switch -- $Priv(selectMode) {\n\
char {\n\
if {[$w compare $cur < $anchorname]} {\n\
set first $cur\n\
set last $anchorname\n\
} else {\n\
set first $anchorname\n\
set last $cur\n\
}\n\
}\n\
word {\n\
if {[$w mark gravity $anchorname] eq \"right\"} {\n\
set first $anchorname\n\
set last \"$anchorname + 1c\"\n\
} else {\n\
set first \"$anchorname - 1c\"\n\
set last $anchorname\n\
}\n\
if {[$w compare $cur < $first]} {\n\
set first $cur\n\
} elseif {[$w compare $cur > $last]} {\n\
set last $cur\n\
}\n\
\n\
set first [TextPrevPos $w \"$first + 1c\" tcl_wordBreakBefore]\n\
set last [TextNextPos $w \"$last - 1c\" tcl_wordBreakAfter]\n\
}\n\
line {\n\
set first \"$anchorname linestart\"\n\
set last \"$anchorname lineend\"\n\
\n\
if {[$w compare $cur < $first]} {\n\
set first \"$cur linestart\"\n\
} elseif {[$w compare $cur > $last]} {\n\
set last \"$cur lineend\"\n\
}\n\
set first [$w index $first]\n\
set last [$w index \"$last + 1c\"]\n\
}\n\
}\n\
if {$Priv(mouseMoved) || ($Priv(selectMode) ne \"char\")} {\n\
$w tag remove sel 0.0 end\n\
$w mark set insert $cur\n\
$w tag add sel $first $last\n\
$w tag remove sel $last end\n\
update idletasks\n\
}\n\
}\n\
\n\
\n\
proc ::tk::TextKeyExtend {w index} {\n\
\n\
set anchorname [tk::TextAnchor $w]\n\
set cur [$w index $index]\n\
if {[catch {$w index $anchorname}]} {\n\
$w mark set $anchorname $cur\n\
}\n\
set anchor [$w index $anchorname]\n\
if {[$w compare $cur < $anchorname]} {\n\
set first $cur\n\
set last $anchorname\n\
} else {\n\
set first $anchorname\n\
set last $cur\n\
}\n\
$w tag remove sel 0.0 $first\n\
$w tag add sel $first $last\n\
$w tag remove sel $last end\n\
}\n\
\n\
\n\
proc ::tk::TextPasteSelection {w x y} {\n\
$w mark set insert [TextClosestGap $w $x $y]\n\
if {![catch {::tk::GetSelection $w PRIMARY} sel]} {\n\
set oldSeparator [$w cget -autoseparators]\n\
if {$oldSeparator} {\n\
$w configure -autoseparators 0\n\
$w edit separator\n\
}\n\
$w insert insert $sel\n\
if {$oldSeparator} {\n\
$w edit separator\n\
$w configure -autoseparators 1\n\
}\n\
}\n\
if {[$w cget -state] eq \"normal\"} {\n\
focus $w\n\
}\n\
}\n\
\n\
\n\
proc ::tk::TextAutoScan {w} {\n\
variable ::tk::Priv\n\
if {![winfo exists $w]} {\n\
return\n\
}\n\
if {$Priv(y) >= [winfo height $w]} {\n\
$w yview scroll [expr {1 + $Priv(y) - [winfo height $w]}] pixels\n\
} elseif {$Priv(y) < 0} {\n\
$w yview scroll [expr {-1 + $Priv(y)}] pixels\n\
} elseif {$Priv(x) >= [winfo width $w]} {\n\
$w xview scroll 2 units\n\
} elseif {$Priv(x) < 0} {\n\
$w xview scroll -2 units\n\
} else {\n\
return\n\
}\n\
TextSelectTo $w $Priv(x) $Priv(y)\n\
set Priv(afterId) [after 50 [list tk::TextAutoScan $w]]\n\
}\n\
\n\
\n\
proc ::tk::TextSetCursor {w pos} {\n\
if {[$w compare $pos == end]} {\n\
set pos {end - 1 chars}\n\
}\n\
$w mark set insert $pos\n\
$w tag remove sel 1.0 end\n\
$w see insert\n\
if {[$w cget -autoseparators]} {\n\
$w edit separator\n\
}\n\
}\n\
\n\
\n\
proc ::tk::TextKeySelect {w new} {\n\
set anchorname [tk::TextAnchor $w]\n\
if {[$w tag nextrange sel 1.0 end] eq \"\"} {\n\
if {[$w compare $new < insert]} {\n\
$w tag add sel $new insert\n\
} else {\n\
$w tag add sel insert $new\n\
}\n\
$w mark set $anchorname insert\n\
} else {\n\
if {[$w compare $new < $anchorname]} {\n\
set first $new\n\
set last $anchorname\n\
} else {\n\
set first $anchorname\n\
set last $new\n\
}\n\
$w tag remove sel 1.0 $first\n\
$w tag add sel $first $last\n\
$w tag remove sel $last end\n\
}\n\
$w mark set insert $new\n\
$w see insert\n\
update idletasks\n\
}\n\
\n\
\n\
proc ::tk::TextResetAnchor {w index} {\n\
if {[$w tag ranges sel] eq \"\"} {\n\
return\n\
}\n\
set anchorname [tk::TextAnchor $w]\n\
set a [$w index $index]\n\
set b [$w index sel.first]\n\
set c [$w index sel.last]\n\
if {[$w compare $a < $b]} {\n\
$w mark set $anchorname sel.last\n\
return\n\
}\n\
if {[$w compare $a > $c]} {\n\
$w mark set $anchorname sel.first\n\
return\n\
}\n\
scan $a \"%d.%d\" lineA chA\n\
scan $b \"%d.%d\" lineB chB\n\
scan $c \"%d.%d\" lineC chC\n\
if {$lineB < $lineC+2} {\n\
set total [string length [$w get $b $c]]\n\
if {$total <= 2} {\n\
return\n\
}\n\
if {[string length [$w get $b $a]] < ($total/2)} {\n\
$w mark set $anchorname sel.last\n\
} else {\n\
$w mark set $anchorname sel.first\n\
}\n\
return\n\
}\n\
if {($lineA-$lineB) < ($lineC-$lineA)} {\n\
$w mark set $anchorname sel.last\n\
} else {\n\
$w mark set $anchorname sel.first\n\
}\n\
}\n\
\n\
\n\
proc ::tk::TextCursorInSelection {w} {\n\
expr {\n\
[llength [$w tag ranges sel]]\n\
&& [$w compare sel.first <= insert]\n\
&& [$w compare sel.last >= insert]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::TextInsert {w s} {\n\
if {$s eq \"\" || [$w cget -state] eq \"disabled\"} {\n\
return\n\
}\n\
set compound 0\n\
if {[TextCursorInSelection $w]} {\n\
set compound [$w cget -autoseparators]\n\
if {$compound} {\n\
$w configure -autoseparators 0\n\
$w edit separator\n\
}\n\
$w delete sel.first sel.last\n\
}\n\
$w insert insert $s\n\
$w see insert\n\
if {$compound} {\n\
$w edit separator\n\
$w configure -autoseparators 1\n\
}\n\
}\n\
\n\
\n\
proc ::tk::TextUpDownLine {w n} {\n\
variable ::tk::Priv\n\
\n\
set i [$w index insert]\n\
if {$Priv(prevPos) ne $i} {\n\
set Priv(textPosOrig) $i\n\
}\n\
set lines [$w count -displaylines $Priv(textPosOrig) $i]\n\
set new [$w index \\\n\
\"$Priv(textPosOrig) + [expr {$lines + $n}] displaylines\"]\n\
if {[$w compare $new == end] \\\n\
|| [$w compare $new == \"insert display linestart\"]} {\n\
set new $i\n\
}\n\
set Priv(prevPos) $new\n\
return $new\n\
}\n\
\n\
\n\
proc ::tk::TextPrevPara {w pos} {\n\
set pos [$w index \"$pos linestart\"]\n\
while {1} {\n\
if {([$w get \"$pos - 1 line\"] eq \"\\n\" && ([$w get $pos] ne \"\\n\")) \\\n\
|| $pos eq \"1.0\"} {\n\
if {[regexp -indices -- {^[ \\t]+(.)} \\\n\
[$w get $pos \"$pos lineend\"] -> index]} {\n\
set pos [$w index \"$pos + [lindex $index 0] chars\"]\n\
}\n\
if {[$w compare $pos != insert] || [lindex [split $pos .] 0]==1} {\n\
return $pos\n\
}\n\
}\n\
set pos [$w index \"$pos - 1 line\"]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::TextNextPara {w start} {\n\
set pos [$w index \"$start linestart + 1 line\"]\n\
while {[$w get $pos] ne \"\\n\"} {\n\
if {[$w compare $pos == end]} {\n\
return [$w index \"end - 1c\"]\n\
}\n\
set pos [$w index \"$pos + 1 line\"]\n\
}\n\
while {[$w get $pos] eq \"\\n\"} {\n\
set pos [$w index \"$pos + 1 line\"]\n\
if {[$w compare $pos == end]} {\n\
return [$w index \"end - 1c\"]\n\
}\n\
}\n\
if {[regexp -indices -- {^[ \\t]+(.)} \\\n\
[$w get $pos \"$pos lineend\"] -> index]} {\n\
return [$w index \"$pos + [lindex $index 0] chars\"]\n\
}\n\
return $pos\n\
}\n\
\n\
\n\
proc ::tk::TextScrollPages {w count} {\n\
set bbox [$w bbox insert]\n\
$w yview scroll $count pages\n\
if {$bbox eq \"\"} {\n\
return [$w index @[expr {[winfo height $w]/2}],0]\n\
}\n\
return [$w index @[lindex $bbox 0],[lindex $bbox 1]]\n\
}\n\
\n\
\n\
proc ::tk::TextTranspose w {\n\
set pos insert\n\
if {[$w compare $pos != \"$pos lineend\"]} {\n\
set pos [$w index \"$pos + 1 char\"]\n\
}\n\
set new [$w get \"$pos - 1 char\"][$w get  \"$pos - 2 char\"]\n\
if {[$w compare \"$pos - 1 char\" == 1.0]} {\n\
return\n\
}\n\
set autosep [$w cget -autoseparators]\n\
if {$autosep} {\n\
$w configure -autoseparators 0\n\
$w edit separator\n\
}\n\
$w delete \"$pos - 2 char\" $pos\n\
$w insert insert $new\n\
$w see insert\n\
if {$autosep} {\n\
$w edit separator\n\
$w configure -autoseparators $autosep\n\
}\n\
}\n\
\n\
\n\
proc ::tk_textCopy w {\n\
if {![catch {set data [$w get sel.first sel.last]}]} {\n\
clipboard clear -displayof $w\n\
clipboard append -displayof $w $data\n\
}\n\
}\n\
\n\
\n\
proc ::tk_textCut w {\n\
if {![catch {set data [$w get sel.first sel.last]}]} {\n\
clipboard clear -displayof $w\n\
clipboard append -displayof $w $data\n\
$w delete sel.first sel.last\n\
}\n\
}\n\
\n\
\n\
proc ::tk_textPaste w {\n\
global tcl_platform\n\
if {![catch {::tk::GetSelection $w CLIPBOARD} sel]} {\n\
set oldSeparator [$w cget -autoseparators]\n\
if {$oldSeparator} {\n\
$w configure -autoseparators 0\n\
$w edit separator\n\
}\n\
if {[tk windowingsystem] ne \"x11\"} {\n\
catch { $w delete sel.first sel.last }\n\
}\n\
$w insert insert $sel\n\
if {$oldSeparator} {\n\
$w edit separator\n\
$w configure -autoseparators 1\n\
}\n\
}\n\
}\n\
\n\
\n\
if {$tcl_platform(platform) eq \"windows\"}  {\n\
proc ::tk::TextNextWord {w start} {\n\
TextNextPos $w [TextNextPos $w $start tcl_endOfWord] \\\n\
tcl_startOfNextWord\n\
}\n\
} else {\n\
proc ::tk::TextNextWord {w start} {\n\
TextNextPos $w $start tcl_endOfWord\n\
}\n\
}\n\
\n\
\n\
proc ::tk::TextNextPos {w start op} {\n\
set text \"\"\n\
set cur $start\n\
while {[$w compare $cur < end]} {\n\
set text $text[$w get -displaychars $cur \"$cur lineend + 1c\"]\n\
set pos [$op $text 0]\n\
if {$pos >= 0} {\n\
return [$w index \"$start + $pos display chars\"]\n\
}\n\
set cur [$w index \"$cur lineend +1c\"]\n\
}\n\
return end\n\
}\n\
\n\
\n\
proc ::tk::TextPrevPos {w start op} {\n\
set text \"\"\n\
set cur $start\n\
while {[$w compare $cur > 0.0]} {\n\
set text [$w get -displaychars \"$cur linestart - 1c\" $cur]$text\n\
set pos [$op $text end]\n\
if {$pos >= 0} {\n\
return [$w index \"$cur linestart - 1c + $pos display chars\"]\n\
}\n\
set cur [$w index \"$cur linestart - 1c\"]\n\
}\n\
return 0.0\n\
}\n\
\n\
\n\
proc ::tk::TextScanMark {w x y} {\n\
variable ::tk::Priv\n\
$w scan mark $x $y\n\
set Priv(x) $x\n\
set Priv(y) $y\n\
set Priv(mouseMoved) 0\n\
}\n\
\n\
\n\
proc ::tk::TextScanDrag {w x y} {\n\
variable ::tk::Priv\n\
if {![info exists Priv(x)]} {\n\
set Priv(x) $x\n\
}\n\
if {![info exists Priv(y)]} {\n\
set Priv(y) $y\n\
}\n\
if {($x != $Priv(x)) || ($y != $Priv(y))} {\n\
set Priv(mouseMoved) 1\n\
}\n\
if {[info exists Priv(mouseMoved)] && $Priv(mouseMoved)} {\n\
$w scan dragto $x $y\n\
}\n\
}\n\
\n\
package require Ttk\n\
\n\
\n\
proc ::tk::IconList {w args} {\n\
IconList_Config $w $args\n\
IconList_Create $w\n\
}\n\
\n\
proc ::tk::IconList_Index {w i} {\n\
upvar #0 ::tk::$w data ::tk::$w:itemList itemList\n\
if {![info exists data(list)]} {\n\
set data(list) {}\n\
}\n\
switch -regexp -- $i {\n\
\"^-?[0-9]+$\" {\n\
if {$i < 0} {\n\
set i 0\n\
}\n\
if {$i >= [llength $data(list)]} {\n\
set i [expr {[llength $data(list)] - 1}]\n\
}\n\
return $i\n\
}\n\
\"^active$\" {\n\
return $data(index,active)\n\
}\n\
\"^anchor$\" {\n\
return $data(index,anchor)\n\
}\n\
\"^end$\" {\n\
return [llength $data(list)]\n\
}\n\
\"@-?[0-9]+,-?[0-9]+\" {\n\
foreach {x y} [scan $i \"@%d,%d\"] {\n\
break\n\
}\n\
set item [$data(canvas) find closest \\\n\
[$data(canvas) canvasx $x] [$data(canvas) canvasy $y]]\n\
return [lindex [$data(canvas) itemcget $item -tags] 1]\n\
}\n\
}\n\
}\n\
\n\
proc ::tk::IconList_Selection {w op args} {\n\
upvar ::tk::$w data\n\
switch -exact -- $op {\n\
\"anchor\" {\n\
if {[llength $args] == 1} {\n\
set data(index,anchor) [tk::IconList_Index $w [lindex $args 0]]\n\
} else {\n\
return $data(index,anchor)\n\
}\n\
}\n\
\"clear\" {\n\
if {[llength $args] == 2} {\n\
foreach {first last} $args {\n\
break\n\
}\n\
} elseif {[llength $args] == 1} {\n\
set first [set last [lindex $args 0]]\n\
} else {\n\
error \"wrong # args: should be [lindex [info level 0] 0] path\\\n\
clear first ?last?\"\n\
}\n\
set first [IconList_Index $w $first]\n\
set last [IconList_Index $w $last]\n\
if {$first > $last} {\n\
set tmp $first\n\
set first $last\n\
set last $tmp\n\
}\n\
set ind 0\n\
foreach item $data(selection) {\n\
if { $item >= $first } {\n\
set first $ind\n\
break\n\
}\n\
incr ind\n\
}\n\
set ind [expr {[llength $data(selection)] - 1}]\n\
for {} {$ind >= 0} {incr ind -1} {\n\
set item [lindex $data(selection) $ind]\n\
if { $item <= $last } {\n\
set last $ind\n\
break\n\
}\n\
}\n\
\n\
if { $first > $last } {\n\
return\n\
}\n\
set data(selection) [lreplace $data(selection) $first $last]\n\
event generate $w <<ListboxSelect>>\n\
IconList_DrawSelection $w\n\
}\n\
\"includes\" {\n\
set index [lsearch -exact $data(selection) [lindex $args 0]]\n\
return [expr {$index != -1}]\n\
}\n\
\"set\" {\n\
if { [llength $args] == 2 } {\n\
foreach {first last} $args {\n\
break\n\
}\n\
} elseif { [llength $args] == 1 } {\n\
set last [set first [lindex $args 0]]\n\
} else {\n\
error \"wrong # args: should be [lindex [info level 0] 0] path\\\n\
set first ?last?\"\n\
}\n\
\n\
set first [IconList_Index $w $first]\n\
set last [IconList_Index $w $last]\n\
if { $first > $last } {\n\
set tmp $first\n\
set first $last\n\
set last $tmp\n\
}\n\
for {set i $first} {$i <= $last} {incr i} {\n\
lappend data(selection) $i\n\
}\n\
set data(selection) [lsort -integer -unique $data(selection)]\n\
event generate $w <<ListboxSelect>>\n\
IconList_DrawSelection $w\n\
}\n\
}\n\
}\n\
\n\
proc ::tk::IconList_CurSelection {w} {\n\
upvar ::tk::$w data\n\
return $data(selection)\n\
}\n\
\n\
proc ::tk::IconList_DrawSelection {w} {\n\
upvar ::tk::$w data\n\
upvar ::tk::$w:itemList itemList\n\
\n\
$data(canvas) delete selection\n\
$data(canvas) itemconfigure selectionText -fill black\n\
$data(canvas) dtag selectionText\n\
set cbg [ttk::style lookup TEntry -selectbackground focus]\n\
set cfg [ttk::style lookup TEntry -selectforeground focus]\n\
foreach item $data(selection) {\n\
set rTag [lindex [lindex $data(list) $item] 2]\n\
foreach {iTag tTag text serial} $itemList($rTag) {\n\
break\n\
}\n\
\n\
set bbox [$data(canvas) bbox $tTag]\n\
$data(canvas) create rect $bbox -fill $cbg -outline $cbg \\\n\
-tags selection\n\
$data(canvas) itemconfigure $tTag -fill $cfg -tags selectionText\n\
}\n\
$data(canvas) lower selection\n\
return\n\
}\n\
\n\
proc ::tk::IconList_Get {w item} {\n\
upvar ::tk::$w data\n\
upvar ::tk::$w:itemList itemList\n\
set rTag [lindex [lindex $data(list) $item] 2]\n\
foreach {iTag tTag text serial} $itemList($rTag) {\n\
break\n\
}\n\
return $text\n\
}\n\
\n\
proc ::tk::IconList_Config {w argList} {\n\
\n\
set specs {\n\
{-command \"\" \"\" \"\"}\n\
{-multiple \"\" \"\" \"0\"}\n\
}\n\
\n\
tclParseConfigSpec ::tk::$w $specs \"\" $argList\n\
}\n\
\n\
proc ::tk::IconList_Create {w} {\n\
upvar ::tk::$w data\n\
\n\
ttk::frame $w\n\
ttk::entry $w.cHull -takefocus 0 -cursor {}\n\
set data(sbar)   [ttk::scrollbar $w.cHull.sbar -orient horizontal -takefocus 0]\n\
catch {$data(sbar) configure -highlightthickness 0}\n\
set data(canvas) [canvas $w.cHull.canvas -highlightthick 0 \\\n\
-width 400 -height 120 -takefocus 1 -background white]\n\
pack $data(sbar) -side bottom -fill x -padx 2 -in $w.cHull -pady {0 2}\n\
pack $data(canvas) -expand yes -fill both -padx 2 -pady {2 0}\n\
pack $w.cHull -expand yes -fill both -ipadx 2 -ipady 2\n\
\n\
$data(sbar) configure -command [list $data(canvas) xview]\n\
$data(canvas) configure -xscrollcommand [list $data(sbar) set]\n\
\n\
set data(maxIW) 1\n\
set data(maxIH) 1\n\
set data(maxTW) 1\n\
set data(maxTH) 1\n\
set data(numItems) 0\n\
set data(noScroll) 1\n\
set data(selection) {}\n\
set data(index,anchor) \"\"\n\
set fg [option get $data(canvas) foreground Foreground]\n\
if {$fg eq \"\"} {\n\
set data(fill) black\n\
} else {\n\
set data(fill) $fg\n\
}\n\
\n\
bind $data(canvas) <Configure>	[list tk::IconList_Arrange $w]\n\
\n\
bind $data(canvas) <1>		[list tk::IconList_Btn1 $w %x %y]\n\
bind $data(canvas) <B1-Motion>	[list tk::IconList_Motion1 $w %x %y]\n\
bind $data(canvas) <B1-Leave>	[list tk::IconList_Leave1 $w %x %y]\n\
bind $data(canvas) <Control-1>	[list tk::IconList_CtrlBtn1 $w %x %y]\n\
bind $data(canvas) <Shift-1>	[list tk::IconList_ShiftBtn1 $w %x %y]\n\
bind $data(canvas) <B1-Enter>	[list tk::CancelRepeat]\n\
bind $data(canvas) <ButtonRelease-1> [list tk::CancelRepeat]\n\
bind $data(canvas) <Double-ButtonRelease-1> \\\n\
[list tk::IconList_Double1 $w %x %y]\n\
\n\
bind $data(canvas) <Control-B1-Motion> {;}\n\
bind $data(canvas) <Shift-B1-Motion> \\\n\
[list tk::IconList_ShiftMotion1 $w %x %y]\n\
\n\
bind $data(canvas) <Up>		[list tk::IconList_UpDown $w -1]\n\
bind $data(canvas) <Down>		[list tk::IconList_UpDown $w  1]\n\
bind $data(canvas) <Left>		[list tk::IconList_LeftRight $w -1]\n\
bind $data(canvas) <Right>		[list tk::IconList_LeftRight $w  1]\n\
bind $data(canvas) <Return>		[list tk::IconList_ReturnKey $w]\n\
bind $data(canvas) <KeyPress>	[list tk::IconList_KeyPress $w %A]\n\
bind $data(canvas) <Control-KeyPress> \";\"\n\
bind $data(canvas) <Alt-KeyPress>	\";\"\n\
\n\
bind $data(canvas) <FocusIn>	[list tk::IconList_FocusIn $w]\n\
bind $data(canvas) <FocusOut>	[list tk::IconList_FocusOut $w]\n\
\n\
return $w\n\
}\n\
\n\
proc ::tk::IconList_AutoScan {w} {\n\
upvar ::tk::$w data\n\
variable ::tk::Priv\n\
\n\
if {![winfo exists $w]} return\n\
set x $Priv(x)\n\
set y $Priv(y)\n\
\n\
if {$data(noScroll)} {\n\
return\n\
}\n\
if {$x >= [winfo width $data(canvas)]} {\n\
$data(canvas) xview scroll 1 units\n\
} elseif {$x < 0} {\n\
$data(canvas) xview scroll -1 units\n\
} elseif {$y >= [winfo height $data(canvas)]} {\n\
} elseif {$y < 0} {\n\
} else {\n\
return\n\
}\n\
\n\
IconList_Motion1 $w $x $y\n\
set Priv(afterId) [after 50 [list tk::IconList_AutoScan $w]]\n\
}\n\
\n\
proc ::tk::IconList_DeleteAll {w} {\n\
upvar ::tk::$w data\n\
upvar ::tk::$w:itemList itemList\n\
\n\
$data(canvas) delete all\n\
unset -nocomplain data(selected) data(rect) data(list) itemList\n\
set data(maxIW) 1\n\
set data(maxIH) 1\n\
set data(maxTW) 1\n\
set data(maxTH) 1\n\
set data(numItems) 0\n\
set data(noScroll) 1\n\
set data(selection) {}\n\
set data(index,anchor) \"\"\n\
$data(sbar) set 0.0 1.0\n\
$data(canvas) xview moveto 0\n\
}\n\
\n\
proc ::tk::IconList_Add {w image items} {\n\
upvar ::tk::$w data\n\
upvar ::tk::$w:itemList itemList\n\
upvar ::tk::$w:textList textList\n\
\n\
foreach text $items {\n\
set iTag [$data(canvas) create image 0 0 -image $image -anchor nw \\\n\
-tags [list icon $data(numItems) item$data(numItems)]]\n\
set tTag [$data(canvas) create text  0 0 -text  $text  -anchor nw \\\n\
-font $data(font) -fill $data(fill) \\\n\
-tags [list text $data(numItems) item$data(numItems)]]\n\
set rTag [$data(canvas) create rect  0 0 0 0 -fill \"\" -outline \"\" \\\n\
-tags [list rect $data(numItems) item$data(numItems)]]\n\
\n\
foreach {x1 y1 x2 y2} [$data(canvas) bbox $iTag] {\n\
break\n\
}\n\
set iW [expr {$x2 - $x1}]\n\
set iH [expr {$y2 - $y1}]\n\
if {$data(maxIW) < $iW} {\n\
set data(maxIW) $iW\n\
}\n\
if {$data(maxIH) < $iH} {\n\
set data(maxIH) $iH\n\
}\n\
\n\
foreach {x1 y1 x2 y2} [$data(canvas) bbox $tTag] {\n\
break\n\
}\n\
set tW [expr {$x2 - $x1}]\n\
set tH [expr {$y2 - $y1}]\n\
if {$data(maxTW) < $tW} {\n\
set data(maxTW) $tW\n\
}\n\
if {$data(maxTH) < $tH} {\n\
set data(maxTH) $tH\n\
}\n\
\n\
lappend data(list) [list $iTag $tTag $rTag $iW $iH $tW \\\n\
$tH $data(numItems)]\n\
set itemList($rTag) [list $iTag $tTag $text $data(numItems)]\n\
set textList($data(numItems)) [string tolower $text]\n\
incr data(numItems)\n\
}\n\
}\n\
\n\
proc ::tk::IconList_Arrange {w} {\n\
upvar ::tk::$w data\n\
\n\
if {![info exists data(list)]} {\n\
if {[info exists data(canvas)] && [winfo exists $data(canvas)]} {\n\
set data(noScroll) 1\n\
$data(sbar) configure -command \"\"\n\
}\n\
return\n\
}\n\
\n\
set W [winfo width  $data(canvas)]\n\
set H [winfo height $data(canvas)]\n\
set pad [expr {[$data(canvas) cget -highlightthickness] + \\\n\
[$data(canvas) cget -bd]}]\n\
if {$pad < 2} {\n\
set pad 2\n\
}\n\
\n\
incr W -[expr {$pad*2}]\n\
incr H -[expr {$pad*2}]\n\
\n\
set dx [expr {$data(maxIW) + $data(maxTW) + 8}]\n\
if {$data(maxTH) > $data(maxIH)} {\n\
set dy $data(maxTH)\n\
} else {\n\
set dy $data(maxIH)\n\
}\n\
incr dy 2\n\
set shift [expr {$data(maxIW) + 4}]\n\
\n\
set x [expr {$pad * 2}]\n\
set y [expr {$pad * 1}] ; # Why * 1 ?\n\
set usedColumn 0\n\
foreach sublist $data(list) {\n\
set usedColumn 1\n\
foreach {iTag tTag rTag iW iH tW tH} $sublist {\n\
break\n\
}\n\
\n\
set i_dy [expr {($dy - $iH)/2}]\n\
set t_dy [expr {($dy - $tH)/2}]\n\
\n\
$data(canvas) coords $iTag $x                    [expr {$y + $i_dy}]\n\
$data(canvas) coords $tTag [expr {$x + $shift}]  [expr {$y + $t_dy}]\n\
$data(canvas) coords $rTag $x $y [expr {$x+$dx}] [expr {$y+$dy}]\n\
\n\
incr y $dy\n\
if {($y + $dy) > $H} {\n\
set y [expr {$pad * 1}] ; # *1 ?\n\
incr x $dx\n\
set usedColumn 0\n\
}\n\
}\n\
\n\
if {$usedColumn} {\n\
set sW [expr {$x + $dx}]\n\
} else {\n\
set sW $x\n\
}\n\
\n\
if {$sW < $W} {\n\
$data(canvas) configure -scrollregion [list $pad $pad $sW $H]\n\
$data(sbar) configure -command \"\"\n\
$data(canvas) xview moveto 0\n\
set data(noScroll) 1\n\
} else {\n\
$data(canvas) configure -scrollregion [list $pad $pad $sW $H]\n\
$data(sbar) configure -command [list $data(canvas) xview]\n\
set data(noScroll) 0\n\
}\n\
\n\
set data(itemsPerColumn) [expr {($H-$pad)/$dy}]\n\
if {$data(itemsPerColumn) < 1} {\n\
set data(itemsPerColumn) 1\n\
}\n\
\n\
IconList_DrawSelection $w\n\
}\n\
\n\
proc ::tk::IconList_Invoke {w} {\n\
upvar ::tk::$w data\n\
\n\
if {$data(-command) ne \"\" && [llength $data(selection)]} {\n\
uplevel #0 $data(-command)\n\
}\n\
}\n\
\n\
proc ::tk::IconList_See {w rTag} {\n\
upvar ::tk::$w data\n\
upvar ::tk::$w:itemList itemList\n\
\n\
if {$data(noScroll)} {\n\
return\n\
}\n\
set sRegion [$data(canvas) cget -scrollregion]\n\
if {$sRegion eq \"\"} {\n\
return\n\
}\n\
\n\
if { $rTag < 0 || $rTag >= [llength $data(list)] } {\n\
return\n\
}\n\
\n\
set bbox [$data(canvas) bbox item$rTag]\n\
set pad [expr {[$data(canvas) cget -highlightthickness] + \\\n\
[$data(canvas) cget -bd]}]\n\
\n\
set x1 [lindex $bbox 0]\n\
set x2 [lindex $bbox 2]\n\
incr x1 -[expr {$pad * 2}]\n\
incr x2 -[expr {$pad * 1}] ; # *1 ?\n\
\n\
set cW [expr {[winfo width $data(canvas)] - $pad*2}]\n\
\n\
set scrollW [expr {[lindex $sRegion 2]-[lindex $sRegion 0]+1}]\n\
set dispX [expr {int([lindex [$data(canvas) xview] 0]*$scrollW)}]\n\
set oldDispX $dispX\n\
\n\
if {($x2 - $dispX) >= $cW} {\n\
set dispX [expr {$x2 - $cW}]\n\
}\n\
if {($x1 - $dispX) < 0} {\n\
set dispX $x1\n\
}\n\
\n\
if {$oldDispX ne $dispX} {\n\
set fraction [expr {double($dispX)/double($scrollW)}]\n\
$data(canvas) xview moveto $fraction\n\
}\n\
}\n\
\n\
proc ::tk::IconList_Btn1 {w x y} {\n\
upvar ::tk::$w data\n\
\n\
focus $data(canvas)\n\
set i [IconList_Index $w @$x,$y]\n\
if {$i eq \"\"} {\n\
return\n\
}\n\
IconList_Selection $w clear 0 end\n\
IconList_Selection $w set $i\n\
IconList_Selection $w anchor $i\n\
}\n\
\n\
proc ::tk::IconList_CtrlBtn1 {w x y} {\n\
upvar ::tk::$w data\n\
\n\
if { $data(-multiple) } {\n\
focus $data(canvas)\n\
set i [IconList_Index $w @$x,$y]\n\
if {$i eq \"\"} {\n\
return\n\
}\n\
if { [IconList_Selection $w includes $i] } {\n\
IconList_Selection $w clear $i\n\
} else {\n\
IconList_Selection $w set $i\n\
IconList_Selection $w anchor $i\n\
}\n\
}\n\
}\n\
\n\
proc ::tk::IconList_ShiftBtn1 {w x y} {\n\
upvar ::tk::$w data\n\
\n\
if { $data(-multiple) } {\n\
focus $data(canvas)\n\
set i [IconList_Index $w @$x,$y]\n\
if {$i eq \"\"} {\n\
return\n\
}\n\
if {[IconList_Index $w anchor] eq \"\"} {\n\
IconList_Selection $w anchor $i\n\
}\n\
IconList_Selection $w clear 0 end\n\
IconList_Selection $w set anchor $i\n\
}\n\
}\n\
\n\
proc ::tk::IconList_Motion1 {w x y} {\n\
variable ::tk::Priv\n\
set Priv(x) $x\n\
set Priv(y) $y\n\
set i [IconList_Index $w @$x,$y]\n\
if {$i eq \"\"} {\n\
return\n\
}\n\
IconList_Selection $w clear 0 end\n\
IconList_Selection $w set $i\n\
}\n\
\n\
proc ::tk::IconList_ShiftMotion1 {w x y} {\n\
upvar ::tk::$w data\n\
variable ::tk::Priv\n\
set Priv(x) $x\n\
set Priv(y) $y\n\
set i [IconList_Index $w @$x,$y]\n\
if {$i eq \"\"} {\n\
return\n\
}\n\
IconList_Selection $w clear 0 end\n\
IconList_Selection $w set anchor $i\n\
}\n\
\n\
proc ::tk::IconList_Double1 {w x y} {\n\
upvar ::tk::$w data\n\
\n\
if {[llength $data(selection)]} {\n\
IconList_Invoke $w\n\
}\n\
}\n\
\n\
proc ::tk::IconList_ReturnKey {w} {\n\
IconList_Invoke $w\n\
}\n\
\n\
proc ::tk::IconList_Leave1 {w x y} {\n\
variable ::tk::Priv\n\
\n\
set Priv(x) $x\n\
set Priv(y) $y\n\
IconList_AutoScan $w\n\
}\n\
\n\
proc ::tk::IconList_FocusIn {w} {\n\
upvar ::tk::$w data\n\
\n\
$w.cHull state focus\n\
if {![info exists data(list)]} {\n\
return\n\
}\n\
\n\
if {[llength $data(selection)]} {\n\
IconList_DrawSelection $w\n\
}\n\
}\n\
\n\
proc ::tk::IconList_FocusOut {w} {\n\
$w.cHull state !focus\n\
IconList_Selection $w clear 0 end\n\
}\n\
\n\
proc ::tk::IconList_UpDown {w amount} {\n\
upvar ::tk::$w data\n\
\n\
if {![info exists data(list)]} {\n\
return\n\
}\n\
\n\
set curr [tk::IconList_CurSelection $w]\n\
if { [llength $curr] == 0 } {\n\
set i 0\n\
} else {\n\
set i [tk::IconList_Index $w anchor]\n\
if {$i eq \"\"} {\n\
return\n\
}\n\
incr i $amount\n\
}\n\
IconList_Selection $w clear 0 end\n\
IconList_Selection $w set $i\n\
IconList_Selection $w anchor $i\n\
IconList_See $w $i\n\
}\n\
\n\
proc ::tk::IconList_LeftRight {w amount} {\n\
upvar ::tk::$w data\n\
\n\
if {![info exists data(list)]} {\n\
return\n\
}\n\
\n\
set curr [IconList_CurSelection $w]\n\
if { [llength $curr] == 0 } {\n\
set i 0\n\
} else {\n\
set i [IconList_Index $w anchor]\n\
if {$i eq \"\"} {\n\
return\n\
}\n\
incr i [expr {$amount*$data(itemsPerColumn)}]\n\
}\n\
IconList_Selection $w clear 0 end\n\
IconList_Selection $w set $i\n\
IconList_Selection $w anchor $i\n\
IconList_See $w $i\n\
}\n\
\n\
\n\
proc ::tk::IconList_KeyPress {w key} {\n\
variable ::tk::Priv\n\
\n\
append Priv(ILAccel,$w) $key\n\
IconList_Goto $w $Priv(ILAccel,$w)\n\
catch {\n\
after cancel $Priv(ILAccel,$w,afterId)\n\
}\n\
set Priv(ILAccel,$w,afterId) [after 500 [list tk::IconList_Reset $w]]\n\
}\n\
\n\
proc ::tk::IconList_Goto {w text} {\n\
upvar ::tk::$w data\n\
upvar ::tk::$w:textList textList\n\
\n\
if {![info exists data(list)]} {\n\
return\n\
}\n\
\n\
if {$text eq \"\" || $data(numItems) == 0} {\n\
return\n\
}\n\
\n\
if {[llength [IconList_CurSelection $w]]} {\n\
set start [IconList_Index $w anchor]\n\
} else {\n\
set start 0\n\
}\n\
\n\
set theIndex -1\n\
set less 0\n\
set len [string length $text]\n\
set len0 [expr {$len-1}]\n\
set i $start\n\
\n\
while {1} {\n\
if {[string equal -nocase -length $len0 $textList($i) $text]} {\n\
set theIndex $i\n\
break\n\
}\n\
incr i\n\
if {$i == $data(numItems)} {\n\
set i 0\n\
}\n\
if {$i == $start} {\n\
break\n\
}\n\
}\n\
\n\
if {$theIndex > -1} {\n\
IconList_Selection $w clear 0 end\n\
IconList_Selection $w set $theIndex\n\
IconList_Selection $w anchor $theIndex\n\
IconList_See $w $theIndex\n\
}\n\
}\n\
\n\
proc ::tk::IconList_Reset {w} {\n\
variable ::tk::Priv\n\
\n\
unset -nocomplain Priv(ILAccel,$w)\n\
}\n\
\n\
\n\
namespace eval ::tk::dialog {}\n\
namespace eval ::tk::dialog::file {\n\
namespace import -force ::tk::msgcat::*\n\
set ::tk::dialog::file::showHiddenBtn 0\n\
set ::tk::dialog::file::showHiddenVar 1\n\
}\n\
\n\
\n\
proc ::tk::dialog::file:: {type args} {\n\
variable ::tk::Priv\n\
set dataName __tk_filedialog\n\
upvar ::tk::dialog::file::$dataName data\n\
\n\
Config $dataName $type $args\n\
\n\
if {$data(-parent) eq \".\"} {\n\
set w .$dataName\n\
} else {\n\
set w $data(-parent).$dataName\n\
}\n\
\n\
if {![winfo exists $w]} {\n\
Create $w TkFDialog\n\
} elseif {[winfo class $w] ne \"TkFDialog\"} {\n\
destroy $w\n\
Create $w TkFDialog\n\
} else {\n\
set data(dirMenuBtn) $w.contents.f1.menu\n\
set data(dirMenu) $w.contents.f1.menu.menu\n\
set data(upBtn) $w.contents.f1.up\n\
set data(icons) $w.contents.icons\n\
set data(ent) $w.contents.f2.ent\n\
set data(typeMenuLab) $w.contents.f2.lab2\n\
set data(typeMenuBtn) $w.contents.f2.menu\n\
set data(typeMenu) $data(typeMenuBtn).m\n\
set data(okBtn) $w.contents.f2.ok\n\
set data(cancelBtn) $w.contents.f2.cancel\n\
set data(hiddenBtn) $w.contents.f2.hidden\n\
SetSelectMode $w $data(-multiple)\n\
}\n\
if {$::tk::dialog::file::showHiddenBtn} {\n\
$data(hiddenBtn) configure -state normal\n\
grid $data(hiddenBtn)\n\
} else {\n\
$data(hiddenBtn) configure -state disabled\n\
grid remove $data(hiddenBtn)\n\
}\n\
\n\
unset -nocomplain data(extUsed)\n\
\n\
\n\
if {[winfo viewable [winfo toplevel $data(-parent)]]} {\n\
wm transient $w $data(-parent)\n\
}\n\
\n\
\n\
trace add variable data(selectPath) write \\\n\
[list ::tk::dialog::file::SetPath $w]\n\
$data(dirMenuBtn) configure \\\n\
-textvariable ::tk::dialog::file::${dataName}(selectPath)\n\
\n\
$data(typeMenu) delete 0 end\n\
$data(typeMenuBtn) configure -state normal -text \"\"\n\
\n\
if {[llength $data(-filetypes)]} {\n\
set initialtype     [lindex $data(-filetypes) 0]\n\
set initialTypeName [lindex $initialtype 0]\n\
if {$data(-typevariable) ne \"\"} {\n\
upvar #0 $data(-typevariable) typeVariable\n\
if {[info exists typeVariable]} {\n\
set initialTypeName $typeVariable\n\
}\n\
}\n\
foreach type $data(-filetypes) {\n\
set title  [lindex $type 0]\n\
set filter [lindex $type 1]\n\
$data(typeMenu) add command -label $title \\\n\
-command [list ::tk::dialog::file::SetFilter $w $type]\n\
if {[string first ${initialTypeName} $title] == 0} {\n\
set initialtype $type\n\
}\n\
}\n\
SetFilter $w $initialtype\n\
$data(typeMenuBtn) configure -state normal\n\
$data(typeMenuLab) configure -state normal\n\
} else {\n\
set data(filter) \"*\"\n\
$data(typeMenuBtn) configure -state disabled -takefocus 0\n\
$data(typeMenuLab) configure -state disabled\n\
}\n\
UpdateWhenIdle $w\n\
\n\
\n\
::tk::PlaceWindow $w widget $data(-parent)\n\
wm title $w $data(-title)\n\
\n\
\n\
::tk::SetFocusGrab $w $data(ent)\n\
$data(ent) delete 0 end\n\
$data(ent) insert 0 $data(selectFile)\n\
$data(ent) selection range 0 end\n\
$data(ent) icursor end\n\
\n\
\n\
vwait ::tk::Priv(selectFilePath)\n\
\n\
::tk::RestoreFocusGrab $w $data(ent) withdraw\n\
\n\
\n\
foreach trace [trace info variable data(selectPath)] {\n\
trace remove variable data(selectPath) [lindex $trace 0] [lindex $trace 1]\n\
}\n\
$data(dirMenuBtn) configure -textvariable {}\n\
\n\
return $Priv(selectFilePath)\n\
}\n\
\n\
proc ::tk::dialog::file::Config {dataName type argList} {\n\
upvar ::tk::dialog::file::$dataName data\n\
\n\
set data(type) $type\n\
\n\
\n\
foreach trace [trace info variable data(selectPath)] {\n\
trace remove variable data(selectPath) [lindex $trace 0] [lindex $trace 1]\n\
}\n\
\n\
set specs {\n\
{-defaultextension \"\" \"\" \"\"}\n\
{-filetypes \"\" \"\" \"\"}\n\
{-initialdir \"\" \"\" \"\"}\n\
{-initialfile \"\" \"\" \"\"}\n\
{-parent \"\" \"\" \".\"}\n\
{-title \"\" \"\" \"\"}\n\
{-typevariable \"\" \"\" \"\"}\n\
}\n\
\n\
if {$type eq \"open\"} {\n\
lappend specs {-multiple \"\" \"\" \"0\"}\n\
}\n\
\n\
if {![info exists data(selectPath)]} {\n\
set data(selectPath) [pwd]\n\
set data(selectFile) \"\"\n\
}\n\
\n\
tclParseConfigSpec ::tk::dialog::file::$dataName $specs \"\" $argList\n\
\n\
if {$data(-title) eq \"\"} {\n\
if {$type eq \"open\"} {\n\
set data(-title) [mc \"Open\"]\n\
} else {\n\
set data(-title) [mc \"Save As\"]\n\
}\n\
}\n\
\n\
if {$data(-initialdir) ne \"\"} {\n\
if {[file isdirectory $data(-initialdir)]} {\n\
set old [pwd]\n\
cd $data(-initialdir)\n\
set data(selectPath) [pwd]\n\
cd $old\n\
} else {\n\
set data(selectPath) [pwd]\n\
}\n\
}\n\
set data(selectFile) $data(-initialfile)\n\
\n\
set data(-filetypes) [::tk::FDGetFileTypes $data(-filetypes)]\n\
\n\
if {![winfo exists $data(-parent)]} {\n\
error \"bad window path name \\\"$data(-parent)\\\"\"\n\
}\n\
\n\
if {$type eq \"save\"} {\n\
set data(-multiple) 0\n\
} elseif {$data(-multiple)} {\n\
set data(-multiple) 1\n\
} else {\n\
set data(-multiple) 0\n\
}\n\
}\n\
\n\
proc ::tk::dialog::file::Create {w class} {\n\
set dataName [lindex [split $w .] end]\n\
upvar ::tk::dialog::file::$dataName data\n\
variable ::tk::Priv\n\
global tk_library\n\
\n\
toplevel $w -class $class\n\
if {[tk windowingsystem] eq \"x11\"} {wm attributes $w -type dialog}\n\
pack [ttk::frame $w.contents] -expand 1 -fill both\n\
\n\
set f1 [ttk::frame $w.contents.f1]\n\
bind [::tk::AmpWidget ttk::label $f1.lab -text [mc \"&Directory:\"]] \\\n\
<<AltUnderlined>> [list focus $f1.menu]\n\
\n\
set data(dirMenuBtn) $f1.menu\n\
if {![info exists data(selectPath)]} {\n\
set data(selectPath) \"\"\n\
}\n\
set data(dirMenu) $f1.menu.menu\n\
ttk::menubutton $f1.menu -menu $data(dirMenu) -direction flush \\\n\
-textvariable [format %s(selectPath) ::tk::dialog::file::$dataName]\n\
[menu $data(dirMenu) -tearoff 0] add radiobutton -label \"\" -variable \\\n\
[format %s(selectPath) ::tk::dialog::file::$dataName]\n\
set data(upBtn) [ttk::button $f1.up]\n\
if {![info exists Priv(updirImage)]} {\n\
set Priv(updirImage) [image create bitmap -data {\n\
#define updir_width 28\n\
#define updir_height 16\n\
static char updir_bits[] = {\n\
0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x40, 0x20, 0x00, 0x00,\n\
0x20, 0x40, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x01, 0x10, 0x00, 0x00, 0x01,\n\
0x10, 0x02, 0x00, 0x01, 0x10, 0x07, 0x00, 0x01, 0x90, 0x0f, 0x00, 0x01,\n\
0x10, 0x02, 0x00, 0x01, 0x10, 0x02, 0x00, 0x01, 0x10, 0x02, 0x00, 0x01,\n\
0x10, 0xfe, 0x07, 0x01, 0x10, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x01,\n\
0xf0, 0xff, 0xff, 0x01};}]\n\
}\n\
$data(upBtn) configure -image $Priv(updirImage)\n\
\n\
$f1.menu configure -takefocus 1;# -highlightthickness 2\n\
\n\
pack $data(upBtn) -side right -padx 4 -fill both\n\
pack $f1.lab -side left -padx 4 -fill both\n\
pack $f1.menu -expand yes -fill both -padx 4\n\
\n\
if {$class eq \"TkFDialog\"} {\n\
if { $data(-multiple) } {\n\
set fNameCaption [mc \"File &names:\"]\n\
} else {\n\
set fNameCaption [mc \"File &name:\"]\n\
}\n\
set fTypeCaption [mc \"Files of &type:\"]\n\
set iconListCommand [list ::tk::dialog::file::OkCmd $w]\n\
} else {\n\
set fNameCaption [mc \"&Selection:\"]\n\
set iconListCommand [list ::tk::dialog::file::chooseDir::DblClick $w]\n\
}\n\
set data(icons) [::tk::IconList $w.contents.icons \\\n\
-command $iconListCommand -multiple $data(-multiple)]\n\
bind $data(icons) <<ListboxSelect>> \\\n\
[list ::tk::dialog::file::ListBrowse $w]\n\
\n\
set f2 [ttk::frame $w.contents.f2]\n\
bind [::tk::AmpWidget ttk::label $f2.lab -text $fNameCaption -anchor e]\\\n\
<<AltUnderlined>> [list focus $f2.ent]\n\
set data(ent) [ttk::entry $f2.ent]\n\
\n\
set ::tk::$w.contents.icons(font) [$data(ent) cget -font]\n\
\n\
if {$class eq \"TkFDialog\"} {\n\
set data(typeMenuLab) [::tk::AmpWidget ttk::label $f2.lab2 \\\n\
-text $fTypeCaption -anchor e]\n\
set data(typeMenuBtn) [ttk::menubutton $f2.menu \\\n\
-menu $f2.menu.m]\n\
set data(typeMenu) [menu $data(typeMenuBtn).m -tearoff 0]\n\
bind $data(typeMenuLab) <<AltUnderlined>> [list \\\n\
focus $data(typeMenuBtn)]\n\
}\n\
\n\
if {$class eq \"TkFDialog\"} {\n\
set text [mc \"Show &Hidden Files and Directories\"]\n\
} else {\n\
set text [mc \"Show &Hidden Directories\"]\n\
}\n\
set data(hiddenBtn) [::tk::AmpWidget ttk::checkbutton $f2.hidden \\\n\
-text $text -state disabled \\\n\
-variable ::tk::dialog::file::showHiddenVar \\\n\
-command [list ::tk::dialog::file::UpdateWhenIdle $w]]\n\
\n\
\n\
set data(okBtn)     [::tk::AmpWidget ttk::button $f2.ok \\\n\
-text [mc \"&OK\"]     -default active];# -pady 3]\n\
bind $data(okBtn) <Destroy> [list ::tk::dialog::file::Destroyed $w]\n\
set data(cancelBtn) [::tk::AmpWidget ttk::button $f2.cancel \\\n\
-text [mc \"&Cancel\"] -default normal];# -pady 3]\n\
\n\
grid $f2.lab $f2.ent $data(okBtn) -padx 4 -pady 3 -sticky ew\n\
grid configure $f2.ent -padx 2\n\
if {$class eq \"TkFDialog\"} {\n\
grid $data(typeMenuLab) $data(typeMenuBtn) $data(cancelBtn) \\\n\
-padx 4 -sticky ew\n\
grid configure $data(typeMenuBtn) -padx 0\n\
grid $data(hiddenBtn) -columnspan 2 -padx 4 -sticky ew\n\
} else {\n\
grid $data(hiddenBtn) - $data(cancelBtn) -padx 4 -sticky ew\n\
}\n\
grid columnconfigure $f2 1 -weight 1\n\
\n\
pack $f1 -side top -fill x -pady 4\n\
pack $f2 -side bottom -pady 4 -fill x\n\
pack $data(icons) -expand yes -fill both -padx 4 -pady 1\n\
\n\
\n\
wm protocol $w WM_DELETE_WINDOW [list ::tk::dialog::file::CancelCmd $w]\n\
$data(upBtn)     configure -command [list ::tk::dialog::file::UpDirCmd $w]\n\
$data(cancelBtn) configure -command [list ::tk::dialog::file::CancelCmd $w]\n\
bind $w <KeyPress-Escape> [list $data(cancelBtn) invoke]\n\
bind $w <Alt-Key> [list tk::AltKeyInDialog $w %A]\n\
\n\
if {$class eq \"TkFDialog\"} {\n\
bind $data(ent) <Return> [list ::tk::dialog::file::ActivateEnt $w]\n\
$data(okBtn)     configure -command [list ::tk::dialog::file::OkCmd $w]\n\
bind $w <Alt-t> [format {\n\
if {[%s cget -state] eq \"normal\"} {\n\
focus %s\n\
}\n\
} $data(typeMenuBtn) $data(typeMenuBtn)]\n\
} else {\n\
set okCmd [list ::tk::dialog::file::chooseDir::OkCmd $w]\n\
bind $data(ent) <Return> $okCmd\n\
$data(okBtn) configure -command $okCmd\n\
bind $w <Alt-s> [list focus $data(ent)]\n\
bind $w <Alt-o> [list $data(okBtn) invoke]\n\
}\n\
bind $w <Alt-h> [list $data(hiddenBtn) invoke]\n\
bind $data(ent) <Tab> [list ::tk::dialog::file::CompleteEnt $w]\n\
\n\
::tk::FocusGroup_Create $w\n\
::tk::FocusGroup_BindIn $w  $data(ent) [list \\\n\
::tk::dialog::file::EntFocusIn $w]\n\
::tk::FocusGroup_BindOut $w $data(ent) [list \\\n\
::tk::dialog::file::EntFocusOut $w]\n\
}\n\
\n\
\n\
proc ::tk::dialog::file::SetSelectMode {w multi} {\n\
set dataName __tk_filedialog\n\
upvar ::tk::dialog::file::$dataName data\n\
if { $multi } {\n\
set fNameCaption [mc \"File &names:\"]\n\
} else {\n\
set fNameCaption [mc \"File &name:\"]\n\
}\n\
set iconListCommand [list ::tk::dialog::file::OkCmd $w]\n\
::tk::SetAmpText $w.contents.f2.lab $fNameCaption\n\
::tk::IconList_Config $data(icons) \\\n\
[list -multiple $multi -command $iconListCommand]\n\
return\n\
}\n\
\n\
proc ::tk::dialog::file::UpdateWhenIdle {w} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
\n\
if {[info exists data(updateId)]} {\n\
return\n\
} else {\n\
set data(updateId) [after idle [list ::tk::dialog::file::Update $w]]\n\
}\n\
}\n\
\n\
proc ::tk::dialog::file::Update {w} {\n\
\n\
if {![winfo exists $w]} {\n\
return\n\
}\n\
set class [winfo class $w]\n\
if {($class ne \"TkFDialog\") && ($class ne \"TkChooseDir\")} {\n\
return\n\
}\n\
\n\
set dataName [winfo name $w]\n\
upvar ::tk::dialog::file::$dataName data\n\
variable ::tk::Priv\n\
global tk_library\n\
unset -nocomplain data(updateId)\n\
\n\
if {![info exists Priv(folderImage)]} {\n\
set Priv(folderImage) [image create photo -data {\n\
R0lGODlhEAAMAKEAAAD//wAAAPD/gAAAACH5BAEAAAAALAAAAAAQAAwAAAIghINhyycvVFsB\n\
QtmS3rjaH1Hg141WaT5ouprt2HHcUgAAOw==}]\n\
set Priv(fileImage)   [image create photo -data {\n\
R0lGODlhDAAMAKEAALLA3AAAAP//8wAAACH5BAEAAAAALAAAAAAMAAwAAAIgRI4Ha+IfWHsO\n\
rSASvJTGhnhcV3EJlo3kh53ltF5nAhQAOw==}]\n\
}\n\
set folder $Priv(folderImage)\n\
set file   $Priv(fileImage)\n\
\n\
set appPWD [pwd]\n\
if {[catch {\n\
cd $data(selectPath)\n\
}]} {\n\
tk_messageBox -type ok -parent $w -icon warning -message \\\n\
[mc \"Cannot change to the directory \\\"%1\\$s\\\".\\nPermission denied.\" $data(selectPath)]\n\
cd $appPWD\n\
return\n\
}\n\
\n\
set entCursor [$data(ent) cget -cursor]\n\
set dlgCursor [$w         cget -cursor]\n\
$data(ent) configure -cursor watch\n\
$w         configure -cursor watch\n\
update idletasks\n\
\n\
::tk::IconList_DeleteAll $data(icons)\n\
\n\
set showHidden $::tk::dialog::file::showHiddenVar\n\
\n\
set cmd [list glob -tails -directory [pwd] -type d -nocomplain *]\n\
if {$showHidden} { lappend cmd .* }\n\
set dirs [lsort -dictionary -unique [eval $cmd]]\n\
set dirList {}\n\
foreach d $dirs {\n\
if {$d eq \".\" || $d eq \"..\"} {\n\
continue\n\
}\n\
lappend dirList $d\n\
}\n\
::tk::IconList_Add $data(icons) $folder $dirList\n\
\n\
if {$class eq \"TkFDialog\"} {\n\
set cmd [list glob -tails -directory [pwd] \\\n\
-type {f b c l p s} -nocomplain]\n\
if {$data(filter) eq \"*\"} {\n\
lappend cmd *\n\
if {$showHidden} {\n\
lappend cmd .*\n\
}\n\
} else {\n\
eval [list lappend cmd] $data(filter)\n\
}\n\
set fileList [lsort -dictionary -unique [eval $cmd]]\n\
::tk::IconList_Add $data(icons) $file $fileList\n\
}\n\
\n\
::tk::IconList_Arrange $data(icons)\n\
\n\
set list \"\"\n\
set dir \"\"\n\
foreach subdir [file split $data(selectPath)] {\n\
set dir [file join $dir $subdir]\n\
lappend list $dir\n\
}\n\
\n\
$data(dirMenu) delete 0 end\n\
set var [format %s(selectPath) ::tk::dialog::file::$dataName]\n\
foreach path $list {\n\
$data(dirMenu) add command -label $path -command [list set $var $path]\n\
}\n\
\n\
cd $appPWD\n\
\n\
if {$class eq \"TkFDialog\"} {\n\
if {$data(type) eq \"open\"} {\n\
::tk::SetAmpText $data(okBtn) [mc \"&Open\"]\n\
} else {\n\
::tk::SetAmpText $data(okBtn) [mc \"&Save\"]\n\
}\n\
}\n\
\n\
$data(ent) configure -cursor $entCursor\n\
$w         configure -cursor $dlgCursor\n\
}\n\
\n\
proc ::tk::dialog::file::SetPathSilently {w path} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
\n\
trace remove variable data(selectPath) write [list ::tk::dialog::file::SetPath $w]\n\
set data(selectPath) $path\n\
trace add variable data(selectPath) write [list ::tk::dialog::file::SetPath $w]\n\
}\n\
\n\
\n\
proc ::tk::dialog::file::SetPath {w name1 name2 op} {\n\
if {[winfo exists $w]} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
UpdateWhenIdle $w\n\
if {[winfo class $w] eq \"TkChooseDir\"} {\n\
$data(ent) delete 0 end\n\
$data(ent) insert end $data(selectPath)\n\
}\n\
}\n\
}\n\
\n\
proc ::tk::dialog::file::SetFilter {w type} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
upvar ::tk::$data(icons) icons\n\
\n\
set data(filterType) $type\n\
set data(filter) [lindex $type 1]\n\
$data(typeMenuBtn) configure -text [lindex $type 0] ;#-indicatoron 1\n\
\n\
if {![info exists data(extUsed)]} {\n\
if {[string length $data(-defaultextension)]} {\n\
set data(extUsed) 1\n\
} else {\n\
set data(extUsed) 0\n\
}\n\
}\n\
\n\
if {!$data(extUsed)} {\n\
set index [lsearch -regexp $data(filter) {^\\*\\.\\w+$}]\n\
if {$index >= 0} {\n\
set data(-defaultextension) \\\n\
[string trimleft [lindex $data(filter) $index] \"*\"]\n\
} else {\n\
set data(-defaultextension) \"\"\n\
}\n\
}\n\
\n\
$icons(sbar) set 0.0 0.0\n\
\n\
UpdateWhenIdle $w\n\
}\n\
\n\
proc ::tk::dialog::file::ResolveFile {context text defaultext {expandEnv 1}} {\n\
set appPWD [pwd]\n\
\n\
set path [JoinFile $context $text]\n\
\n\
if {\n\
![file isdirectory $path] && ([file ext $path] eq \"\") &&\n\
![string match {$*} [file tail $path]]\n\
} then {\n\
set path \"$path$defaultext\"\n\
}\n\
\n\
if {[catch {file exists $path}]} {\n\
return [list ERROR $path \"\"]\n\
}\n\
\n\
if {[file exists $path]} {\n\
if {[file isdirectory $path]} {\n\
if {[catch {cd $path}]} {\n\
return [list CHDIR $path \"\"]\n\
}\n\
set directory [pwd]\n\
set file \"\"\n\
set flag OK\n\
cd $appPWD\n\
} else {\n\
if {[catch {cd [file dirname $path]}]} {\n\
return [list CHDIR [file dirname $path] \"\"]\n\
}\n\
set directory [pwd]\n\
set file [file tail $path]\n\
set flag OK\n\
cd $appPWD\n\
}\n\
} else {\n\
set dirname [file dirname $path]\n\
if {[file exists $dirname]} {\n\
if {[catch {cd $dirname}]} {\n\
return [list CHDIR $dirname \"\"]\n\
}\n\
set directory [pwd]\n\
cd $appPWD\n\
set file [file tail $path]\n\
if {$expandEnv && [string match {$*} $file]} {\n\
set var [string range $file 1 end]\n\
if {[info exist ::env($var)]} {\n\
return [ResolveFile $context $::env($var) $defaultext 0]\n\
}\n\
}\n\
if {[regexp {[*?]} $file]} {\n\
set flag PATTERN\n\
} else {\n\
set flag FILE\n\
}\n\
} else {\n\
set directory $dirname\n\
set file [file tail $path]\n\
set flag PATH\n\
if {$expandEnv && [string match {$*} $file]} {\n\
set var [string range $file 1 end]\n\
if {[info exist ::env($var)]} {\n\
return [ResolveFile $context $::env($var) $defaultext 0]\n\
}\n\
}\n\
}\n\
}\n\
\n\
return [list $flag $directory $file]\n\
}\n\
\n\
\n\
proc ::tk::dialog::file::EntFocusIn {w} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
\n\
if {[$data(ent) get] ne \"\"} {\n\
$data(ent) selection range 0 end\n\
$data(ent) icursor end\n\
} else {\n\
$data(ent) selection clear\n\
}\n\
\n\
if {[winfo class $w] eq \"TkFDialog\"} {\n\
if {$data(type) eq \"open\"} {\n\
::tk::SetAmpText $data(okBtn) [mc \"&Open\"]\n\
} else {\n\
::tk::SetAmpText $data(okBtn) [mc \"&Save\"]\n\
}\n\
}\n\
}\n\
\n\
proc ::tk::dialog::file::EntFocusOut {w} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
\n\
$data(ent) selection clear\n\
}\n\
\n\
\n\
proc ::tk::dialog::file::ActivateEnt {w} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
\n\
set text [$data(ent) get]\n\
if {$data(-multiple)} {\n\
foreach t $text {\n\
VerifyFileName $w $t\n\
}\n\
} else {\n\
VerifyFileName $w $text\n\
}\n\
}\n\
\n\
proc ::tk::dialog::file::VerifyFileName {w filename} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
\n\
set list [ResolveFile $data(selectPath) $filename $data(-defaultextension)]\n\
foreach {flag path file} $list {\n\
break\n\
}\n\
\n\
switch -- $flag {\n\
OK {\n\
if {$file eq \"\"} {\n\
set data(selectPath) $path\n\
$data(ent) delete 0 end\n\
} else {\n\
SetPathSilently $w $path\n\
if {$data(-multiple)} {\n\
lappend data(selectFile) $file\n\
} else {\n\
set data(selectFile) $file\n\
}\n\
Done $w\n\
}\n\
}\n\
PATTERN {\n\
set data(selectPath) $path\n\
set data(filter) $file\n\
}\n\
FILE {\n\
if {$data(type) eq \"open\"} {\n\
tk_messageBox -icon warning -type ok -parent $w \\\n\
-message [mc \"File \\\"%1\\$s\\\"  does not exist.\" \\\n\
[file join $path $file]]\n\
$data(ent) selection range 0 end\n\
$data(ent) icursor end\n\
} else {\n\
SetPathSilently $w $path\n\
if {$data(-multiple)} {\n\
lappend data(selectFile) $file\n\
} else {\n\
set data(selectFile) $file\n\
}\n\
Done $w\n\
}\n\
}\n\
PATH {\n\
tk_messageBox -icon warning -type ok -parent $w \\\n\
-message [mc \"Directory \\\"%1\\$s\\\" does not exist.\" $path]\n\
$data(ent) selection range 0 end\n\
$data(ent) icursor end\n\
}\n\
CHDIR {\n\
tk_messageBox -type ok -parent $w -icon warning -message  \\\n\
[mc \"Cannot change to the directory\\\n\
\\\"%1\\$s\\\".\\nPermission denied.\" $path]\n\
$data(ent) selection range 0 end\n\
$data(ent) icursor end\n\
}\n\
ERROR {\n\
tk_messageBox -type ok -parent $w -icon warning -message \\\n\
[mc \"Invalid file name \\\"%1\\$s\\\".\" $path]\n\
$data(ent) selection range 0 end\n\
$data(ent) icursor end\n\
}\n\
}\n\
}\n\
\n\
proc ::tk::dialog::file::InvokeBtn {w key} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
\n\
if {[$data(okBtn) cget -text] eq $key} {\n\
$data(okBtn) invoke\n\
}\n\
}\n\
\n\
proc ::tk::dialog::file::UpDirCmd {w} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
\n\
if {$data(selectPath) ne \"/\"} {\n\
set data(selectPath) [file dirname $data(selectPath)]\n\
}\n\
}\n\
\n\
proc ::tk::dialog::file::JoinFile {path file} {\n\
if {[string match {~*} $file] && [file exists $path/$file]} {\n\
return [file join $path ./$file]\n\
} else {\n\
return [file join $path $file]\n\
}\n\
}\n\
\n\
proc ::tk::dialog::file::OkCmd {w} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
\n\
set filenames {}\n\
foreach item [::tk::IconList_CurSelection $data(icons)] {\n\
lappend filenames [::tk::IconList_Get $data(icons) $item]\n\
}\n\
\n\
if {([llength $filenames] && !$data(-multiple)) || \\\n\
($data(-multiple) && ([llength $filenames] == 1))} {\n\
set filename [lindex $filenames 0]\n\
set file [JoinFile $data(selectPath) $filename]\n\
if {[file isdirectory $file]} {\n\
ListInvoke $w [list $filename]\n\
return\n\
}\n\
}\n\
\n\
ActivateEnt $w\n\
}\n\
\n\
proc ::tk::dialog::file::CancelCmd {w} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
variable ::tk::Priv\n\
\n\
bind $data(okBtn) <Destroy> {}\n\
set Priv(selectFilePath) \"\"\n\
}\n\
\n\
proc ::tk::dialog::file::Destroyed {w} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
variable ::tk::Priv\n\
\n\
set Priv(selectFilePath) \"\"\n\
}\n\
\n\
proc ::tk::dialog::file::ListBrowse {w} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
\n\
set text {}\n\
foreach item [::tk::IconList_CurSelection $data(icons)] {\n\
lappend text [::tk::IconList_Get $data(icons) $item]\n\
}\n\
if {[llength $text] == 0} {\n\
return\n\
}\n\
if {$data(-multiple)} {\n\
set newtext {}\n\
foreach file $text {\n\
set fullfile [JoinFile $data(selectPath) $file]\n\
if { ![file isdirectory $fullfile] } {\n\
lappend newtext $file\n\
}\n\
}\n\
set text $newtext\n\
set isDir 0\n\
} else {\n\
set text [lindex $text 0]\n\
set file [JoinFile $data(selectPath) $text]\n\
set isDir [file isdirectory $file]\n\
}\n\
if {!$isDir} {\n\
$data(ent) delete 0 end\n\
$data(ent) insert 0 $text\n\
\n\
if {[winfo class $w] eq \"TkFDialog\"} {\n\
if {$data(type) eq \"open\"} {\n\
::tk::SetAmpText $data(okBtn) [mc \"&Open\"]\n\
} else {\n\
::tk::SetAmpText $data(okBtn) [mc \"&Save\"]\n\
}\n\
}\n\
} elseif {[winfo class $w] eq \"TkFDialog\"} {\n\
::tk::SetAmpText $data(okBtn) [mc \"&Open\"]\n\
}\n\
}\n\
\n\
proc ::tk::dialog::file::ListInvoke {w filenames} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
\n\
if {[llength $filenames] == 0} {\n\
return\n\
}\n\
\n\
set file [JoinFile $data(selectPath) [lindex $filenames 0]]\n\
\n\
set class [winfo class $w]\n\
if {$class eq \"TkChooseDir\" || [file isdirectory $file]} {\n\
set appPWD [pwd]\n\
if {[catch {cd $file}]} {\n\
tk_messageBox -type ok -parent $w -icon warning -message \\\n\
[mc \"Cannot change to the directory \\\"%1\\$s\\\".\\nPermission denied.\" $file]\n\
} else {\n\
cd $appPWD\n\
set data(selectPath) $file\n\
}\n\
} else {\n\
if {$data(-multiple)} {\n\
set data(selectFile) $filenames\n\
} else {\n\
set data(selectFile) $file\n\
}\n\
Done $w\n\
}\n\
}\n\
\n\
proc ::tk::dialog::file::Done {w {selectFilePath \"\"}} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
variable ::tk::Priv\n\
\n\
if {$selectFilePath eq \"\"} {\n\
if {$data(-multiple)} {\n\
set selectFilePath {}\n\
foreach f $data(selectFile) {\n\
lappend selectFilePath [JoinFile $data(selectPath) $f]\n\
}\n\
} else {\n\
set selectFilePath [JoinFile $data(selectPath) $data(selectFile)]\n\
}\n\
\n\
set Priv(selectFile) $data(selectFile)\n\
set Priv(selectPath) $data(selectPath)\n\
\n\
if {($data(type) eq \"save\") && [file exists $selectFilePath]} {\n\
set reply [tk_messageBox -icon warning -type yesno -parent $w \\\n\
-message [mc \"File \\\"%1\\$s\\\" already exists.\\nDo you want\\\n\
to overwrite it?\" $selectFilePath]]\n\
if {$reply eq \"no\"} {\n\
return\n\
}\n\
}\n\
if {[info exists data(-typevariable)] && $data(-typevariable) ne \"\"\n\
&& [info exists data(-filetypes)] && [llength $data(-filetypes)]\n\
&& [info exists data(filterType)] && $data(filterType) ne \"\"} {\n\
upvar #0 $data(-typevariable) typeVariable\n\
set typeVariable [lindex $data(filterType) 0]\n\
}\n\
}\n\
bind $data(okBtn) <Destroy> {}\n\
set Priv(selectFilePath) $selectFilePath\n\
}\n\
\n\
proc ::tk::dialog::file::CompleteEnt {w} {\n\
upvar ::tk::dialog::file::[winfo name $w] data\n\
set f [$data(ent) get]\n\
if {$data(-multiple)} {\n\
if {[catch {llength $f} len] || $len != 1} {\n\
return -code break\n\
}\n\
set f [lindex $f 0]\n\
}\n\
\n\
set globF [list glob -tails -directory $data(selectPath) \\\n\
-type {f b c l p s} -nocomplain]\n\
set globD [list glob -tails -directory $data(selectPath) -type d \\\n\
-nocomplain *]\n\
if {$data(filter) eq \"*\"} {\n\
lappend globF *\n\
if {$::tk::dialog::file::showHiddenVar} {\n\
lappend globF .*\n\
lappend globD .*\n\
}\n\
if {[winfo class $w] eq \"TkFDialog\"} {\n\
set files [lsort -dictionary -unique [{*}$globF]]\n\
} else {\n\
set files {}\n\
}\n\
set dirs [lsort -dictionary -unique [{*}$globD]]\n\
} else {\n\
if {$::tk::dialog::file::showHiddenVar} {\n\
lappend globD .*\n\
}\n\
if {[winfo class $w] eq \"TkFDialog\"} {\n\
set files [lsort -dictionary -unique [{*}$globF {*}$data(filter)]]\n\
} else {\n\
set files {}\n\
}\n\
set dirs [lsort -dictionary -unique [{*}$globD]]\n\
}\n\
set dirs [lsearch -all -not -exact -inline $dirs .]\n\
set dirs [lsearch -all -not -exact -inline $dirs ..]\n\
set dirs2 {}\n\
foreach d $dirs {lappend dirs2 $d/}\n\
\n\
set targets [concat \\\n\
[lsearch -glob -all -inline $files $f*] \\\n\
[lsearch -glob -all -inline $dirs2 $f*]]\n\
\n\
if {[llength $targets] == 1} {\n\
set f [lindex $targets 0]\n\
} elseif {$f in $targets || [llength $targets] == 0} {\n\
if {[string length $f] > 0} {\n\
bell\n\
}\n\
return\n\
} elseif {[llength $targets] > 1} {\n\
if {[string length $f] == 0} {\n\
return\n\
}\n\
set t0 [lindex $targets 0]\n\
for {set len [string length $t0]} {$len>0} {} {\n\
set allmatch 1\n\
foreach s $targets {\n\
if {![string equal -length $len $s $t0]} {\n\
set allmatch 0\n\
break\n\
}\n\
}\n\
incr len -1\n\
if {$allmatch} break\n\
}\n\
set f [string range $t0 0 $len]\n\
}\n\
\n\
if {$data(-multiple)} {\n\
set f [list $f]\n\
}\n\
$data(ent) delete 0 end\n\
$data(ent) insert 0 $f\n\
return -code break\n\
}\n\
\n\
namespace eval ::tk::dialog {}\n\
\n\
image create bitmap ::tk::dialog::b1 -foreground black \\\n\
-data \"#define b1_width 32\\n#define b1_height 32\n\
static unsigned char q1_bits[] = {\n\
0x00, 0xf8, 0x1f, 0x00, 0x00, 0x07, 0xe0, 0x00, 0xc0, 0x00, 0x00, 0x03,\n\
0x20, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x10,\n\
0x04, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x40,\n\
0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80,\n\
0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80,\n\
0x01, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x40,\n\
0x04, 0x00, 0x00, 0x20, 0x08, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x08,\n\
0x60, 0x00, 0x00, 0x04, 0x80, 0x03, 0x80, 0x03, 0x00, 0x0c, 0x78, 0x00,\n\
0x00, 0x30, 0x04, 0x00, 0x00, 0x40, 0x04, 0x00, 0x00, 0x40, 0x04, 0x00,\n\
0x00, 0x80, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};\"\n\
image create bitmap ::tk::dialog::b2 -foreground white \\\n\
-data \"#define b2_width 32\\n#define b2_height 32\n\
static unsigned char b2_bits[] = {\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0xff, 0xff, 0x00,\n\
0xc0, 0xff, 0xff, 0x03, 0xe0, 0xff, 0xff, 0x07, 0xf0, 0xff, 0xff, 0x0f,\n\
0xf8, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0x3f,\n\
0xfe, 0xff, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0x7f,\n\
0xfe, 0xff, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0x7f, 0xfe, 0xff, 0xff, 0x7f,\n\
0xfe, 0xff, 0xff, 0x7f, 0xfc, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0x3f,\n\
0xf8, 0xff, 0xff, 0x1f, 0xf0, 0xff, 0xff, 0x0f, 0xe0, 0xff, 0xff, 0x07,\n\
0x80, 0xff, 0xff, 0x03, 0x00, 0xfc, 0x7f, 0x00, 0x00, 0xf0, 0x07, 0x00,\n\
0x00, 0xc0, 0x03, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x80, 0x03, 0x00,\n\
0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};\"\n\
image create bitmap ::tk::dialog::q -foreground blue \\\n\
-data \"#define q_width 32\\n#define q_height 32\n\
static unsigned char q_bits[] = {\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00,\n\
0x00, 0x10, 0x0f, 0x00, 0x00, 0x18, 0x1e, 0x00, 0x00, 0x38, 0x1e, 0x00,\n\
0x00, 0x38, 0x1e, 0x00, 0x00, 0x10, 0x0f, 0x00, 0x00, 0x80, 0x07, 0x00,\n\
0x00, 0xc0, 0x01, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00,\n\
0x00, 0xe0, 0x01, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};\"\n\
image create bitmap ::tk::dialog::i -foreground blue \\\n\
-data \"#define i_width 32\\n#define i_height 32\n\
static unsigned char i_bits[] = {\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0xe0, 0x01, 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0xf0, 0x03, 0x00,\n\
0x00, 0xe0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0xf8, 0x03, 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0xe0, 0x03, 0x00,\n\
0x00, 0xe0, 0x03, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0xe0, 0x03, 0x00,\n\
0x00, 0xe0, 0x03, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0xf0, 0x07, 0x00,\n\
0x00, 0xf8, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};\"\n\
image create bitmap ::tk::dialog::w1 -foreground black \\\n\
-data \"#define w1_width 32\\n#define w1_height 32\n\
static unsigned char w1_bits[] = {\n\
0x00, 0x80, 0x01, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x20, 0x04, 0x00,\n\
0x00, 0x10, 0x04, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x08, 0x08, 0x00,\n\
0x00, 0x08, 0x10, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x04, 0x20, 0x00,\n\
0x00, 0x02, 0x20, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x01, 0x40, 0x00,\n\
0x00, 0x01, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x01,\n\
0x40, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x02, 0x20, 0x00, 0x00, 0x02,\n\
0x20, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x04, 0x10, 0x00, 0x00, 0x08,\n\
0x08, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x10, 0x04, 0x00, 0x00, 0x10,\n\
0x04, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x40,\n\
0x01, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x20,\n\
0xfc, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00};\"\n\
image create bitmap ::tk::dialog::w2 -foreground yellow \\\n\
-data \"#define w2_width 32\\n#define w2_height 32\n\
static unsigned char w2_bits[] = {\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0xc0, 0x03, 0x00,\n\
0x00, 0xe0, 0x03, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xf0, 0x07, 0x00,\n\
0x00, 0xf0, 0x0f, 0x00, 0x00, 0xf8, 0x0f, 0x00, 0x00, 0xf8, 0x1f, 0x00,\n\
0x00, 0xfc, 0x1f, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0xfe, 0x3f, 0x00,\n\
0x00, 0xfe, 0x7f, 0x00, 0x00, 0xff, 0x7f, 0x00, 0x00, 0xff, 0xff, 0x00,\n\
0x80, 0xff, 0xff, 0x00, 0x80, 0xff, 0xff, 0x01, 0xc0, 0xff, 0xff, 0x01,\n\
0xc0, 0xff, 0xff, 0x03, 0xe0, 0xff, 0xff, 0x03, 0xe0, 0xff, 0xff, 0x07,\n\
0xf0, 0xff, 0xff, 0x07, 0xf0, 0xff, 0xff, 0x0f, 0xf8, 0xff, 0xff, 0x0f,\n\
0xf8, 0xff, 0xff, 0x1f, 0xfc, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0x3f,\n\
0xfe, 0xff, 0xff, 0x3f, 0xfe, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0x1f,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};\"\n\
image create bitmap ::tk::dialog::w3 -foreground black \\\n\
-data \"#define w3_width 32\\n#define w3_height 32\n\
static unsigned char w3_bits[] = {\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0xc0, 0x03, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00,\n\
0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00,\n\
0x00, 0xc0, 0x03, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0xc0, 0x03, 0x00,\n\
0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0xc0, 0x03, 0x00,\n\
0x00, 0xc0, 0x03, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};\"\n\
\n\
proc ::tk::MessageBox {args} {\n\
global tcl_platform tk_strictMotif\n\
variable ::tk::Priv\n\
\n\
set w ::tk::PrivMsgBox\n\
upvar $w data\n\
\n\
set specs {\n\
{-default \"\" \"\" \"\"}\n\
{-detail \"\" \"\" \"\"}\n\
{-icon \"\" \"\" \"info\"}\n\
{-message \"\" \"\" \"\"}\n\
{-parent \"\" \"\" .}\n\
{-title \"\" \"\" \" \"}\n\
{-type \"\" \"\" \"ok\"}\n\
}\n\
\n\
tclParseConfigSpec $w $specs \"\" $args\n\
\n\
if {[lsearch -exact {info warning error question} $data(-icon)] == -1} {\n\
error \"bad -icon value \\\"$data(-icon)\\\": must be error, info, question, or warning\"\n\
}\n\
set windowingsystem [tk windowingsystem]\n\
if {$windowingsystem eq \"aqua\"} {\n\
switch -- $data(-icon) {\n\
\"error\"     {set data(-icon) \"stop\"}\n\
\"warning\"   {set data(-icon) \"caution\"}\n\
\"info\"      {set data(-icon) \"note\"}\n\
}\n\
option add *Dialog*background systemDialogBackgroundActive widgetDefault\n\
option add *Dialog*Button.highlightBackground \\\n\
systemDialogBackgroundActive widgetDefault\n\
}\n\
\n\
if {![winfo exists $data(-parent)]} {\n\
error \"bad window path name \\\"$data(-parent)\\\"\"\n\
}\n\
\n\
switch -- $data(-type) {\n\
abortretryignore { \n\
set names [list abort retry ignore]\n\
set labels [list &Abort &Retry &Ignore]\n\
set cancel abort\n\
}\n\
ok {\n\
set names [list ok]\n\
set labels {&OK}\n\
set cancel ok\n\
}\n\
okcancel {\n\
set names [list ok cancel]\n\
set labels [list &OK &Cancel]\n\
set cancel cancel\n\
}\n\
retrycancel {\n\
set names [list retry cancel]\n\
set labels [list &Retry &Cancel]\n\
set cancel cancel\n\
}\n\
yesno {\n\
set names [list yes no]\n\
set labels [list &Yes &No]\n\
set cancel no\n\
}\n\
yesnocancel {\n\
set names [list yes no cancel]\n\
set labels [list &Yes &No &Cancel]\n\
set cancel cancel\n\
}\n\
default {\n\
error \"bad -type value \\\"$data(-type)\\\": must be\\\n\
abortretryignore, ok, okcancel, retrycancel,\\\n\
yesno, or yesnocancel\"\n\
}\n\
}\n\
\n\
set buttons {}\n\
foreach name $names lab $labels {\n\
lappend buttons [list $name -text [mc $lab]]\n\
}\n\
\n\
\n\
if {$data(-default) eq \"\"} {\n\
set data(-default) [lindex [lindex $buttons 0] 0]\n\
}\n\
\n\
set valid 0\n\
foreach btn $buttons {\n\
if {[lindex $btn 0] eq $data(-default)} {\n\
set valid 1\n\
break\n\
}\n\
}\n\
if {!$valid} {\n\
error \"invalid default button \\\"$data(-default)\\\"\"\n\
}\n\
\n\
if {$data(-parent) ne \".\"} {\n\
set w $data(-parent).__tk__messagebox\n\
} else {\n\
set w .__tk__messagebox\n\
}\n\
\n\
set bg [ttk::style lookup . -background]\n\
\n\
\n\
catch {destroy $w}\n\
toplevel $w -class Dialog -bg $bg\n\
wm title $w $data(-title)\n\
wm iconname $w Dialog\n\
wm protocol $w WM_DELETE_WINDOW [list $w.$cancel invoke]\n\
\n\
if {[winfo viewable [winfo toplevel $data(-parent)]] } {\n\
wm transient $w $data(-parent)\n\
}\n\
\n\
if {$windowingsystem eq \"aqua\"} {\n\
::tk::unsupported::MacWindowStyle style $w moveableModal {}\n\
} elseif {$windowingsystem eq \"x11\"} {\n\
wm attributes $w -type dialog\n\
}\n\
\n\
ttk::frame $w.bot;# -background $bg\n\
grid anchor $w.bot center\n\
pack $w.bot -side bottom -fill both\n\
ttk::frame $w.top;# -background $bg\n\
pack $w.top -side top -fill both -expand 1\n\
if {$windowingsystem ne \"aqua\"} {\n\
}\n\
\n\
\n\
option add *Dialog.msg.wrapLength 3i widgetDefault\n\
option add *Dialog.dtl.wrapLength 3i widgetDefault\n\
option add *Dialog.msg.font TkCaptionFont widgetDefault\n\
option add *Dialog.dtl.font TkDefaultFont widgetDefault\n\
\n\
ttk::label $w.msg -anchor nw -justify left -text $data(-message)\n\
if {$data(-detail) ne \"\"} {\n\
ttk::label $w.dtl -anchor nw -justify left -text $data(-detail)\n\
}\n\
if {$data(-icon) ne \"\"} {\n\
if {$windowingsystem eq \"aqua\"\n\
|| ([winfo depth $w] < 4) || $tk_strictMotif} {\n\
label $w.bitmap -bitmap $data(-icon);# -background $bg\n\
} else {\n\
canvas $w.bitmap -width 32 -height 32 -highlightthickness 0 \\\n\
-background $bg\n\
switch $data(-icon) {\n\
error {\n\
$w.bitmap create oval 0 0 31 31 -fill red -outline black\n\
$w.bitmap create line 9 9 23 23 -fill white -width 4\n\
$w.bitmap create line 9 23 23 9 -fill white -width 4\n\
}\n\
info {\n\
$w.bitmap create image 0 0 -anchor nw \\\n\
-image ::tk::dialog::b1\n\
$w.bitmap create image 0 0 -anchor nw \\\n\
-image ::tk::dialog::b2\n\
$w.bitmap create image 0 0 -anchor nw \\\n\
-image ::tk::dialog::i\n\
}\n\
question {\n\
$w.bitmap create image 0 0 -anchor nw \\\n\
-image ::tk::dialog::b1\n\
$w.bitmap create image 0 0 -anchor nw \\\n\
-image ::tk::dialog::b2\n\
$w.bitmap create image 0 0 -anchor nw \\\n\
-image ::tk::dialog::q\n\
}\n\
default {\n\
$w.bitmap create image 0 0 -anchor nw \\\n\
-image ::tk::dialog::w1\n\
$w.bitmap create image 0 0 -anchor nw \\\n\
-image ::tk::dialog::w2\n\
$w.bitmap create image 0 0 -anchor nw \\\n\
-image ::tk::dialog::w3\n\
}\n\
}\n\
}\n\
}\n\
grid $w.bitmap $w.msg -in $w.top -sticky news -padx 2m -pady 2m\n\
grid columnconfigure $w.top 1 -weight 1\n\
if {$data(-detail) ne \"\"} {\n\
grid ^ $w.dtl -in $w.top -sticky news -padx 2m -pady {0 2m}\n\
grid rowconfigure $w.top 1 -weight 1\n\
} else {\n\
grid rowconfigure $w.top 0 -weight 1\n\
}\n\
\n\
\n\
set i 0\n\
foreach but $buttons {\n\
set name [lindex $but 0]\n\
set opts [lrange $but 1 end]\n\
if {![llength $opts]} {\n\
set capName [string toupper $name 0]\n\
set opts [list -text $capName]\n\
}\n\
\n\
eval [list tk::AmpWidget ttk::button $w.$name] $opts \\\n\
[list -command [list set tk::Priv(button) $name]]\n\
\n\
if {$name eq $data(-default)} {\n\
$w.$name configure -default active\n\
} else {\n\
$w.$name configure -default normal\n\
}\n\
grid $w.$name -in $w.bot -row 0 -column $i -padx 3m -pady 2m -sticky ew\n\
grid columnconfigure $w.bot $i -uniform buttons\n\
if {$windowingsystem eq \"aqua\"} {\n\
set tmp [string tolower $name]\n\
if {$tmp eq \"ok\" || $tmp eq \"cancel\" || $tmp eq \"yes\" ||\n\
$tmp eq \"no\" || $tmp eq \"abort\" || $tmp eq \"retry\" ||\n\
$tmp eq \"ignore\"} {\n\
grid columnconfigure $w.bot $i -minsize 90\n\
}\n\
grid configure $w.$name -pady 7\n\
}\n\
incr i\n\
\n\
}\n\
bind $w <Alt-Key> [list ::tk::AltKeyInDialog $w %A]\n\
\n\
if {$data(-default) ne \"\"} {\n\
bind $w <FocusIn> {\n\
if {[winfo class %W] in \"Button TButton\"} {\n\
%W configure -default active\n\
}\n\
}\n\
bind $w <FocusOut> {\n\
if {[winfo class %W] in \"Button TButton\"} {\n\
%W configure -default normal\n\
}\n\
}\n\
}\n\
\n\
\n\
bind $w <Return> {\n\
if {[winfo class %W] in \"Button TButton\"} {\n\
%W invoke\n\
}\n\
}\n\
\n\
bind $w <Escape> [list $w.$cancel invoke]\n\
\n\
bind $w.msg <Destroy> [list set tk::Priv(button) $cancel]\n\
\n\
\n\
::tk::PlaceWindow $w widget $data(-parent)\n\
\n\
\n\
if {$data(-default) ne \"\"} {\n\
set focus $w.$data(-default)\n\
} else {\n\
set focus $w\n\
}\n\
::tk::SetFocusGrab $w $focus\n\
\n\
\n\
vwait ::tk::Priv(button)\n\
set result $Priv(button)\n\
\n\
::tk::RestoreFocusGrab $w $focus\n\
\n\
return $result\n\
}\n\
\n\
\n\
proc ::tk_optionMenu {w varName firstValue args} {\n\
upvar #0 $varName var\n\
\n\
if {![info exists var]} {\n\
set var $firstValue\n\
}\n\
menubutton $w -textvariable $varName -indicatoron 1 -menu $w.menu \\\n\
-relief raised -highlightthickness 1 -anchor c \\\n\
-direction flush\n\
menu $w.menu -tearoff 0\n\
$w.menu add radiobutton -label $firstValue -variable $varName\n\
foreach i $args {\n\
$w.menu add radiobutton -label $i -variable $varName\n\
}\n\
return $w.menu\n\
}\n\
\n\
\n\
if {[tk windowingsystem] eq \"x11\"} {\n\
\n\
bind Scrollbar <Enter> {\n\
if {$tk_strictMotif} {\n\
set tk::Priv(activeBg) [%W cget -activebackground]\n\
%W configure -activebackground [%W cget -background]\n\
}\n\
%W activate [%W identify %x %y]\n\
}\n\
bind Scrollbar <Motion> {\n\
%W activate [%W identify %x %y]\n\
}\n\
\n\
\n\
bind Scrollbar <Leave> {\n\
if {$tk_strictMotif && [info exists tk::Priv(activeBg)]} {\n\
%W configure -activebackground $tk::Priv(activeBg)\n\
}\n\
%W activate {}\n\
}\n\
bind Scrollbar <1> {\n\
tk::ScrollButtonDown %W %x %y\n\
}\n\
bind Scrollbar <B1-Motion> {\n\
tk::ScrollDrag %W %x %y\n\
}\n\
bind Scrollbar <B1-B2-Motion> {\n\
tk::ScrollDrag %W %x %y\n\
}\n\
bind Scrollbar <ButtonRelease-1> {\n\
tk::ScrollButtonUp %W %x %y\n\
}\n\
bind Scrollbar <B1-Leave> {\n\
}\n\
bind Scrollbar <B1-Enter> {\n\
}\n\
bind Scrollbar <2> {\n\
tk::ScrollButton2Down %W %x %y\n\
}\n\
bind Scrollbar <B1-2> {\n\
}\n\
bind Scrollbar <B2-1> {\n\
}\n\
bind Scrollbar <B2-Motion> {\n\
tk::ScrollDrag %W %x %y\n\
}\n\
bind Scrollbar <ButtonRelease-2> {\n\
tk::ScrollButtonUp %W %x %y\n\
}\n\
bind Scrollbar <B1-ButtonRelease-2> {\n\
}\n\
bind Scrollbar <B2-ButtonRelease-1> {\n\
}\n\
bind Scrollbar <B2-Leave> {\n\
}\n\
bind Scrollbar <B2-Enter> {\n\
}\n\
bind Scrollbar <Control-1> {\n\
tk::ScrollTopBottom %W %x %y\n\
}\n\
bind Scrollbar <Control-2> {\n\
tk::ScrollTopBottom %W %x %y\n\
}\n\
\n\
bind Scrollbar <Up> {\n\
tk::ScrollByUnits %W v -1\n\
}\n\
bind Scrollbar <Down> {\n\
tk::ScrollByUnits %W v 1\n\
}\n\
bind Scrollbar <Control-Up> {\n\
tk::ScrollByPages %W v -1\n\
}\n\
bind Scrollbar <Control-Down> {\n\
tk::ScrollByPages %W v 1\n\
}\n\
bind Scrollbar <Left> {\n\
tk::ScrollByUnits %W h -1\n\
}\n\
bind Scrollbar <Right> {\n\
tk::ScrollByUnits %W h 1\n\
}\n\
bind Scrollbar <Control-Left> {\n\
tk::ScrollByPages %W h -1\n\
}\n\
bind Scrollbar <Control-Right> {\n\
tk::ScrollByPages %W h 1\n\
}\n\
bind Scrollbar <Prior> {\n\
tk::ScrollByPages %W hv -1\n\
}\n\
bind Scrollbar <Next> {\n\
tk::ScrollByPages %W hv 1\n\
}\n\
bind Scrollbar <Home> {\n\
tk::ScrollToPos %W 0\n\
}\n\
bind Scrollbar <End> {\n\
tk::ScrollToPos %W 1\n\
}\n\
}\n\
if {[tk windowingsystem] eq \"aqua\"} {\n\
bind Scrollbar <MouseWheel> {\n\
tk::ScrollByUnits %W v [expr {- (%D)}]\n\
}\n\
bind Scrollbar <Option-MouseWheel> {\n\
tk::ScrollByUnits %W v [expr {-10 * (%D)}]\n\
}\n\
bind Scrollbar <Shift-MouseWheel> {\n\
tk::ScrollByUnits %W h [expr {- (%D)}]\n\
}\n\
bind Scrollbar <Shift-Option-MouseWheel> {\n\
tk::ScrollByUnits %W h [expr {-10 * (%D)}]\n\
}\n\
}\n\
\n\
proc tk::ScrollButtonDown {w x y} {\n\
variable ::tk::Priv\n\
set Priv(relief) [$w cget -activerelief]\n\
$w configure -activerelief sunken\n\
set element [$w identify $x $y]\n\
if {$element eq \"slider\"} {\n\
ScrollStartDrag $w $x $y\n\
} else {\n\
ScrollSelect $w $element initial\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScrollButtonUp {w x y} {\n\
variable ::tk::Priv\n\
tk::CancelRepeat\n\
if {[info exists Priv(relief)]} {\n\
$w configure -activerelief $Priv(relief)\n\
ScrollEndDrag $w $x $y\n\
$w activate [$w identify $x $y]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScrollSelect {w element repeat} {\n\
variable ::tk::Priv\n\
if {![winfo exists $w]} return\n\
switch -- $element {\n\
\"arrow1\"	{ScrollByUnits $w hv -1}\n\
\"trough1\"	{ScrollByPages $w hv -1}\n\
\"trough2\"	{ScrollByPages $w hv 1}\n\
\"arrow2\"	{ScrollByUnits $w hv 1}\n\
default		{return}\n\
}\n\
if {$repeat eq \"again\"} {\n\
set Priv(afterId) [after [$w cget -repeatinterval] \\\n\
[list tk::ScrollSelect $w $element again]]\n\
} elseif {$repeat eq \"initial\"} {\n\
set delay [$w cget -repeatdelay]\n\
if {$delay > 0} {\n\
set Priv(afterId) [after $delay \\\n\
[list tk::ScrollSelect $w $element again]]\n\
}\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScrollStartDrag {w x y} {\n\
variable ::tk::Priv\n\
\n\
if {[$w cget -command] eq \"\"} {\n\
return\n\
}\n\
set Priv(pressX) $x\n\
set Priv(pressY) $y\n\
set Priv(initValues) [$w get]\n\
set iv0 [lindex $Priv(initValues) 0]\n\
if {[llength $Priv(initValues)] == 2} {\n\
set Priv(initPos) $iv0\n\
} elseif {$iv0 == 0} {\n\
set Priv(initPos) 0.0\n\
} else {\n\
set Priv(initPos) [expr {(double([lindex $Priv(initValues) 2])) \\\n\
/ [lindex $Priv(initValues) 0]}]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScrollDrag {w x y} {\n\
variable ::tk::Priv\n\
\n\
if {$Priv(initPos) eq \"\"} {\n\
return\n\
}\n\
set delta [$w delta [expr {$x - $Priv(pressX)}] [expr {$y - $Priv(pressY)}]]\n\
if {[$w cget -jump]} {\n\
if {[llength $Priv(initValues)] == 2} {\n\
$w set [expr {[lindex $Priv(initValues) 0] + $delta}] \\\n\
[expr {[lindex $Priv(initValues) 1] + $delta}]\n\
} else {\n\
set delta [expr {round($delta * [lindex $Priv(initValues) 0])}]\n\
eval [list $w] set [lreplace $Priv(initValues) 2 3 \\\n\
[expr {[lindex $Priv(initValues) 2] + $delta}] \\\n\
[expr {[lindex $Priv(initValues) 3] + $delta}]]\n\
}\n\
} else {\n\
ScrollToPos $w [expr {$Priv(initPos) + $delta}]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScrollEndDrag {w x y} {\n\
variable ::tk::Priv\n\
\n\
if {$Priv(initPos) eq \"\"} {\n\
return\n\
}\n\
if {[$w cget -jump]} {\n\
set delta [$w delta [expr {$x - $Priv(pressX)}] \\\n\
[expr {$y - $Priv(pressY)}]]\n\
ScrollToPos $w [expr {$Priv(initPos) + $delta}]\n\
}\n\
set Priv(initPos) \"\"\n\
}\n\
\n\
\n\
proc ::tk::ScrollByUnits {w orient amount} {\n\
set cmd [$w cget -command]\n\
if {$cmd eq \"\" || ([string first \\\n\
[string index [$w cget -orient] 0] $orient] < 0)} {\n\
return\n\
}\n\
set info [$w get]\n\
if {[llength $info] == 2} {\n\
uplevel #0 $cmd scroll $amount units\n\
} else {\n\
uplevel #0 $cmd [expr {[lindex $info 2] + $amount}]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScrollByPages {w orient amount} {\n\
set cmd [$w cget -command]\n\
if {$cmd eq \"\" || ([string first \\\n\
[string index [$w cget -orient] 0] $orient] < 0)} {\n\
return\n\
}\n\
set info [$w get]\n\
if {[llength $info] == 2} {\n\
uplevel #0 $cmd scroll $amount pages\n\
} else {\n\
uplevel #0 $cmd [expr {[lindex $info 2] + $amount*([lindex $info 1] - 1)}]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScrollToPos {w pos} {\n\
set cmd [$w cget -command]\n\
if {$cmd eq \"\"} {\n\
return\n\
}\n\
set info [$w get]\n\
if {[llength $info] == 2} {\n\
uplevel #0 $cmd moveto $pos\n\
} else {\n\
uplevel #0 $cmd [expr {round([lindex $info 0]*$pos)}]\n\
}\n\
}\n\
\n\
\n\
proc ::tk::ScrollTopBottom {w x y} {\n\
variable ::tk::Priv\n\
set element [$w identify $x $y]\n\
if {[string match *1 $element]} {\n\
ScrollToPos $w 0\n\
} elseif {[string match *2 $element]} {\n\
ScrollToPos $w 1\n\
}\n\
\n\
\n\
set Priv(relief) [$w cget -activerelief]\n\
}\n\
\n\
\n\
proc ::tk::ScrollButton2Down {w x y} {\n\
variable ::tk::Priv\n\
set element [$w identify $x $y]\n\
if {[string match {arrow[12]} $element]} {\n\
ScrollButtonDown $w $x $y\n\
return\n\
}\n\
ScrollToPos $w [$w fraction $x $y]\n\
set Priv(relief) [$w cget -activerelief]\n\
\n\
\n\
update idletasks\n\
$w configure -activerelief sunken\n\
$w activate slider\n\
ScrollStartDrag $w $x $y\n\
}\n\
";
#include "tclcl.h"
EmbeddedTcl et_tk(code);
