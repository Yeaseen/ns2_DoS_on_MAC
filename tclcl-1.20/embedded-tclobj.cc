static char code[] = "\n\
Class InitObject\n\
\n\
Object instproc init-vars {args} {\n\
$self init-default-vars [$self info class]\n\
\n\
set shadow_args \"\"\n\
for {} {$args != \"\"} {set args [lrange $args 2 end]} {\n\
set key [lindex $args 0]\n\
set val [lindex $args 1]\n\
if {$val != \"\" && [string match {-[A-z]*} $key]} {\n\
set cmd [string range $key 1 end]\n\
if ![catch \"$self $cmd $val\"] {\n\
continue\n\
}\n\
}\n\
lappend shadow_args $key $val\n\
}\n\
return $shadow_args\n\
}\n\
\n\
Object instproc init-default-vars {classes} {\n\
foreach cl $classes {\n\
if {$cl == \"Object\"} continue\n\
$self init-default-vars \"[$cl info superclass]\"\n\
foreach var [$cl info vars] {\n\
if [catch \"$self set $var\"] {\n\
$self set $var [$cl set $var]\n\
}\n\
}\n\
}\n\
}\n\
\n\
\n\
\n\
Class SplitObject\n\
SplitObject set id 0\n\
\n\
SplitObject instproc init args {\n\
$self next\n\
if [catch \"$self create-shadow $args\"] {\n\
error \"__FAILED_SHADOW_OBJECT_\" \"\"\n\
}\n\
}\n\
\n\
SplitObject instproc set args {\n\
set var [lindex $args 0]\n\
$self instvar -parse-part1 $var\n\
if {[llength $args] == 1} {\n\
return [subst $[subst $var]]\n\
} else {\n\
return [set $var [lindex $args 1]]\n\
}\n\
}\n\
\n\
SplitObject instproc destroy {} {\n\
$self delete-shadow\n\
$self next\n\
}\n\
\n\
SplitObject proc getid {} {\n\
$self instvar id\n\
incr id\n\
return _o$id\n\
}\n\
\n\
SplitObject proc is-class cl {\n\
if [catch \"SplitObject info subclass $cl\" v] {\n\
return 0\n\
}\n\
return $v\n\
}\n\
\n\
SplitObject instproc unknown args {\n\
if [catch \"$self cmd $args\" ret] {\n\
set cls [$self info class]\n\
global errorInfo\n\
set savedInfo $errorInfo\n\
error \"error when calling class $cls: $args\" $savedInfo\n\
}\n\
return $ret\n\
}\n\
\n\
proc new { className args } {\n\
set o [SplitObject getid]\n\
if [catch \"$className create $o $args\" msg] {\n\
if [string match \"__FAILED_SHADOW_OBJECT_\" $msg] {\n\
delete $o\n\
return \"\"\n\
}\n\
global errorInfo\n\
error \"class $className: constructor failed: $msg\" $errorInfo\n\
}\n\
return $o\n\
}\n\
\n\
proc delete o {\n\
$o delete_tkvar\n\
$o destroy\n\
}\n\
\n\
SplitObject proc register className {\n\
set classes [split $className /]\n\
set parent SplitObject\n\
set path \"\"\n\
set sep \"\"\n\
foreach cl $classes {\n\
set path $path$sep$cl\n\
if ![$self is-class $path] {\n\
Class $path -superclass $parent\n\
}\n\
set sep /\n\
set parent $path\n\
}\n\
}\n\
\n\
SplitObject instproc warn-instvar item {\n\
$self instvar issuedWarning\n\
if ![info exists issuedWarning($item)] {\n\
set issuedWarning($item) 1\n\
puts stderr \"warning: no class variable $item\\n\"\n\
$self instvar SplitObject_issued_undeclared_warning\n\
if ![info exists SplitObject_issued_undeclared_warning] {\n\
puts stderr \"\\tsee tcl-object.tcl in tclcl for info about this warning.\\n\"\n\
set SplitObject_issued_undeclared_warning 1\n\
}\n\
}\n\
}\n\
\n\
SplitObject instproc init-instvar var {\n\
set cl [$self info class]\n\
while { \"$cl\" != \"\" && \"$cl\" != \"SplitObject\" } {\n\
foreach c $cl {\n\
if ![catch \"$c set $var\" val] {\n\
$self set $var $val\n\
return\n\
}\n\
}\n\
set parents \"\"\n\
foreach c $cl {\n\
if { $cl != \"SplitObject\" && $cl != \"Object\" } {\n\
set parents \"$parents [$c info superclass]\"\n\
}\n\
}\n\
set cl $parents\n\
}\n\
$self warn-instvar [$self info class]::$var\n\
}\n\
\n\
proc tkerror msg {\n\
global errorInfo\n\
puts -nonewline \"$msg: \"\n\
puts $errorInfo\n\
exit 1\n\
}\n\
\n\
proc bgerror msg {\n\
global errorInfo\n\
puts -nonewline \"$msg: \"\n\
puts $errorInfo\n\
exit 1\n\
}\n\
\n\
Object instproc public args {\n\
eval $self instproc $args\n\
}\n\
\n\
Object instproc private args {\n\
eval $self instproc $args\n\
}\n\
\n\
Object instproc proc.public args {\n\
eval $self proc $args\n\
}\n\
\n\
Object instproc proc.private args {\n\
eval $self proc $args\n\
}\n\
\n\
Object instproc proc.invoke { arglist body args } {\n\
$self proc invokeproc_ $arglist $body\n\
eval [list $self] invokeproc_ $args\n\
}\n\
\n\
\n\
\n\
Object instproc tkvar args {\n\
foreach var $args {\n\
if { [llength $var] > 1 } {\n\
set varname [lindex $var 1]\n\
set var [lindex $var 0]\n\
} else {\n\
set varname $var\n\
}\n\
uplevel upvar #0 $self/$var $varname\n\
}\n\
}\n\
\n\
Object instproc tkvarname var {\n\
return $self/$var\n\
}\n\
\n\
Object instproc delete_tkvar { } {\n\
set fullname [$self tkvarname foo]\n\
regexp \"(.*)foo$\" $fullname dummy prefix\n\
foreach global [info globals \"$prefix*\"] {\n\
global $global\n\
unset $global\n\
}\n\
}\n\
\n\
Object instproc info_tkvar { pattern } {\n\
set pattern [$self tkvarname $pattern]\n\
return [info globals $pattern]\n\
}\n\
\n\
proc TclObject args {\n\
return [eval SplitObject $args]\n\
}\n\
\n\
proc SplitObjectCompare {a b} {\n\
set o1 [string range $a 2 end]\n\
set o2 [string range $b 2 end]\n\
if {$o1 < $o2} {\n\
return -1\n\
} elseif {$o1 == $o2} {\n\
return 0\n\
} else {\n\
return 1\n\
}\n\
}\n\
\n\
Object instproc extract-var varname {\n\
set aidx [string first \"(\" $varname]\n\
if { $aidx >= 0 } {\n\
string range $varname 0 [incr aidx -1]\n\
} else {\n\
set varname\n\
}\n\
}\n\
\n\
Object instproc add-to-list {list elem} {\n\
$self instvar [$self extract-var $list]\n\
set ret 0\n\
if ![info exists $list] {\n\
set $list $elem\n\
set ret 1\n\
} elseif { [lsearch [set $list] $elem] < 0 } {\n\
lappend $list $elem\n\
set ret 1\n\
}\n\
set ret\n\
}\n\
\n\
Object instproc remove-from-list {list elem} {\n\
$self instvar [$self extract-var $list]\n\
set wtag \"$self: remove $elem from $list failed\"\n\
set ret  0\n\
if ![info exists $list] {\n\
warn \"$wtag: list does not exist\"\n\
} else {\n\
set k [lsearch [set $list] $elem]\n\
if { $k < 0 } {\n\
warn \"$wtag: element does not exist\"\n\
} else {\n\
set $list [lreplace [set $list] $k $k]\n\
set ret 1\n\
}\n\
}\n\
set ret\n\
}\n\
\n\
\n\
\n\
Class Import\n\
\n\
\n\
Import public init { } {\n\
$self next\n\
$self set use_http_cache_ 1\n\
}\n\
\n\
\n\
Import public import { args } {\n\
$self instvar import_dirs_ table_\n\
\n\
if { ![info exists import_dirs_] } {\n\
$self init_table\n\
}\n\
\n\
$self consistency_check\n\
\n\
\n\
foreach item $args {\n\
if [info exists table_($item)] {\n\
set file_list $table_($item)\n\
foreach file $table_($item) {\n\
if { [set msg [$self source_file $file]]!=\"\"} {\n\
error \"could not source $file for\\\n\
$item:\\n$msg\"\n\
}\n\
}\n\
} else {\n\
\n\
set list {}\n\
foreach dir $import_dirs_ {\n\
lappend list [$self file join $dir \\\n\
[$self class_to_file \\\n\
$item].mash]\n\
}\n\
\n\
set imported 0\n\
foreach filename $list {\n\
if { [$self source_file $filename] == \"\" } {\n\
set imported 1\n\
break\n\
}\n\
}\n\
\n\
if { ! $imported } {\n\
error \"don't know how to import $item\\n    not\\\n\
mapped in: $import_dirs_\\\n\
\\n    and not found in default\\\n\
locations: $list\"\n\
}\n\
}\n\
}\n\
}\n\
\n\
\n\
\n\
Import public override_importTable_mapping { object file_list } {\n\
$self instvar overrideTable_ import_dirs_\n\
\n\
if { [info exists import_dirs_] } {\n\
puts stderr \"warning: ignoring \\\"override_importTable_mapping\\\n\
$object $file_list\\\" \\n\\\n\
It is illegal to modify the internal table \\\n\
after the first call to import.\"\n\
return\n\
}\n\
\n\
if { [info exists overrideTable_($object)] } {\n\
unset overrideTable_($object)\n\
}\n\
\n\
foreach file $file_list {\n\
set fname [$self condense_into_absolute_filename \\\n\
[$self file join [pwd] $file]]\n\
lappend overrideTable_($object) $fname\n\
}\n\
}\n\
\n\
\n\
Import proc.private redefine_unknown {} {\n\
\n\
rename unknown unknown.orig\n\
proc unknown { args } {\n\
if { ![catch \"eval {unknown.orig} $args\" m] } { \n\
return\n\
}\n\
$self instvar autoimport_\n\
if { [info exists autoimport_] && $autoimport_ } {\n\
really_import [lindex $args 0]\n\
} else {\n\
error \"$m\" \n\
}\n\
}\n\
\n\
Import proc.private redefine_unknown {} {}\n\
}\n\
\n\
Import proc.public enable_autoimport {} {\n\
import Class Object mashutils\n\
\n\
Import set autoimport_ 1\n\
$self redefine_unknown\n\
return\n\
}\n\
\n\
\n\
Import proc.public disable_autoimport {} {\n\
Import set autoimport_ 0\n\
return\n\
}\n\
\n\
Import disable_autoimport \n\
\n\
\n\
\n\
Import private init_table { } {\n\
$self instvar import_dirs_\n\
global env\n\
\n\
if { ![info exists env(TCLCL_IMPORT_DIRS)] } {\n\
set env(TCLCL_IMPORT_DIRS) .\n\
}\n\
\n\
set import_dirs_ \"\"\n\
foreach dir [$self smart_parse_env_var $env(TCLCL_IMPORT_DIRS)] {\n\
lappend import_dirs_ [$self condense_to_absolute_filename $dir]\n\
}\n\
\n\
set dirs [$self find_import_dirs $import_dirs_]\n\
\n\
$self make_table $dirs\n\
}\n\
\n\
\n\
Import private make_table { dirs } {\n\
foreach d $dirs {\n\
$self read_dir $d\n\
}\n\
\n\
$self incorporate_table_overrides\n\
}\n\
\n\
\n\
Import private incorporate_table_overrides {} {\n\
$self instvar overrideTable_ table_\n\
\n\
foreach object [array names overrideTable_] {\n\
set table_($object) $overrideTable_($object)\n\
}\n\
}\n\
\n\
\n\
Import private find_import_dirs { dirs } {\n\
set list {}\n\
foreach dir $dirs {\n\
set importLocation [$self file join $dir importLocation]\n\
set r [$self file readable $importLocation]\n\
if [lindex $r 0] {\n\
set lines [$self read_file_into_list $importLocation]\n\
foreach line $lines {\n\
lappend list [$self \\\n\
condense_to_absolute_filename \\\n\
[$self file join $dir $line]]\n\
}\n\
if { [lindex $r 1] != {} } {\n\
unset [lindex $r 1]\n\
}\n\
} else {\n\
lappend list $dir\n\
}\n\
}\n\
\n\
$self instvar last_modified_\n\
set dirs \"\"\n\
foreach d $list {\n\
set import_table [$self file join $d importTable]\n\
set last_modified_($import_table) -1\n\
set r [$self file readable $import_table]\n\
if [lindex $r 0] {\n\
lappend dirs $d\n\
if { [lindex $r 1] != {} } {\n\
unset [lindex $r 1]\n\
}\n\
}\n\
}\n\
\n\
\n\
return $dirs\n\
}\n\
\n\
\n\
Import private read_dir { dir } {\n\
$self instvar table_ classes_mapped_ last_modified_\n\
\n\
set importTableFile [$self condense_to_absolute_filename \\\n\
[$self file join $dir importTable]]\n\
set last_modified_($importTableFile) -1\n\
\n\
set lines [$self read_file_into_list $importTableFile]\n\
\n\
foreach line $lines {\n\
set index [lindex $line 0]\n\
set fname [$self condense_to_absolute_filename \\\n\
[$self file join $dir [lindex $line 1]]]\n\
set last_modified [string trim [lindex $line 2]]\n\
\n\
if [info exists classes_mapped_($index)] {\n\
continue\n\
}\n\
\n\
if {[info exists table_($index)]} {\n\
if {-1!=[lsearch -exact $table_($index) $fname]} {\n\
continue\n\
}\n\
}\n\
\n\
lappend table_($index) $fname\n\
if { $last_modified!={} } {\n\
set last_modified_($fname) $last_modified\n\
}\n\
\n\
set this_mappings($index) 1\n\
}\n\
\n\
foreach index [array names this_mappings] {\n\
set classes_mapped_($index) 1\n\
}\n\
}\n\
\n\
\n\
Import private smart_parse_env_var { env_value } {\n\
set env $env_value\n\
while {[string length [set env [string trim $env \":\"]]] != 0 } {\n\
if [regexp {^([^:]+)://([^:/]+)(:([0-9]+))?(/[^:]*)} \\\n\
$env url protocol server x port trailingpath] {\n\
lappend list $url\n\
regsub {([^:]+)://([^:/]+)(:([0-9]+))?(/[^:]*)} \\\n\
$env {} env\n\
} else {\n\
regexp {^[^:]+} $env dir\n\
lappend list $dir\n\
regsub {^[^:]+} $env {} env\n\
}\n\
}\n\
\n\
return $list\n\
}\n\
\n\
\n\
Import private consistency_check { } {\n\
global env\n\
$self instvar orig_val_\n\
\n\
if { ![info exists orig_val_] } {\n\
set orig_val_ $env(TCLCL_IMPORT_DIRS)\n\
}\n\
\n\
if { $env(TCLCL_IMPORT_DIRS) != $orig_val_ } {\n\
puts stderr \"warning: ignoring modification to\\\n\
env(TCLCL_IMPORT_DIRS)\\nit is illegal to\\\n\
modify this after the first call to the\\\n\
import procedure.\"\n\
}\n\
}\n\
\n\
\n\
Import private source_file { file } {\n\
set file_readable_result [$self file readable $file]\n\
set file_readable [lindex $file_readable_result 0]\n\
\n\
if { $file_readable } {\n\
set read_token [lindex $file_readable_result 1] \n\
$self source $file $read_token\n\
if { $read_token!={} } { unset $read_token }\n\
return \"\"\n\
} else {\n\
return [lindex $file_readable_result 1]\n\
}\n\
}\n\
\n\
\n\
Import private source { file { read_token {} } } {\n\
$self instvar loaded_ uniq_num_\n\
\n\
if { ![info exists uniq_num_] } {\n\
set uniq_num_ 0\n\
}\n\
\n\
set file [$self condense_to_absolute_filename $file]\n\
if [info exists loaded_($file)] {\n\
return \n\
}\n\
set loaded_($file) 1\n\
\n\
incr uniq_num_	\n\
uplevel \\#0 \"rename source source.$uniq_num_\"\n\
uplevel \\#0 \"proc source {args} \\{ $self source \\$args \\}\"\n\
\n\
if [$self is_http_url $file] {\n\
set buffer [$self read_url $file $read_token]\n\
\n\
if [catch \"uplevel \\#0 {$buffer}\" errormsg] {\n\
global errorInfo\n\
error \"error in $file: $errormsg\\n$errorInfo\\n\\n\"\n\
}\n\
} else {\n\
\n\
if [catch \"uplevel \\#0 source.orig $file\" errormsg] {\n\
global errorInfo\n\
error \"error in $file: $errormsg\\n$errorInfo\\n\\n\"\n\
}\n\
}\n\
\n\
uplevel \\#0 {rename source {}}\n\
uplevel \\#0 \"rename source.$uniq_num_ source\"\n\
incr uniq_num_ -1\n\
}\n\
\n\
\n\
Import private enable { args } {\n\
}\n\
\n\
\n\
Import private class_to_file c {\n\
regsub -all -- \"/\" $c \"-\" filename\n\
return $filename\n\
}\n\
\n\
\n\
\n\
Import private is_http_url { name } {\n\
if [regexp {([^:]+)://([^:/]+)(:([0-9]+))?(/.*)} $name url protocol \\\n\
server x port trailingpath] {\n\
if { ![info exists protocol] } {\n\
return 0\n\
} else {\n\
return [regexp -nocase {http} $protocol]\n\
}\n\
} else {\n\
return 0\n\
}\n\
}\n\
\n\
\n\
Import private read_url { url {token {}} } {\n\
$self instvar use_http_cache_ cache_ last_modified_\n\
if { $token == {} } {\n\
\n\
if $use_http_cache_ {\n\
if { ![info exists cache_] } {\n\
set cache_ [new HTTPCache]\n\
}\n\
\n\
if [info exists last_modified_($url)] {\n\
set buffer [$cache_ get $url \\\n\
$last_modified_($url)]\n\
} else {\n\
set buffer [$cache_ get $url]\n\
}\n\
if { $buffer==\"\" } { unset buffer }\n\
}\n\
\n\
if { ![info exists buffer] } {\n\
set token [Http geturl $url]\n\
if { [lindex [set code [::http::code $token]] 1] \\\n\
!= 200 } {\n\
error \"couldn't read \\\"$url\\\": no such file \\\n\
or directory (HTTP code $code)\"\n\
}\n\
set buffer [::http::data $token]\n\
unset $token\n\
\n\
if $use_http_cache_ {\n\
if { ![info exists cache_] } {\n\
set cache_ [new HTTPCache]\n\
}\n\
\n\
$cache_ put $url $buffer\n\
}\n\
}\n\
} else {\n\
set buffer [::http::data $token]\n\
}\n\
return $buffer\n\
}\n\
\n\
\n\
Import private condense_to_absolute_filename { name } {\n\
\n\
return $name\n\
\n\
set before_cd [pwd]\n\
while { ![catch \"file readlink $filename\"] } {\n\
set filename [file readlink $filename]\n\
}\n\
set dirname [$self file dirname $filename]\n\
set tailname [file tail $filename]\n\
set condensed_name $filename\n\
if { ![catch \"cd $dirname\"] } {\n\
set condensed_name [ufile join [pwd] $tailname]\n\
}\n\
cd $before_cd\n\
return $condensed_name\n\
}\n\
\n\
\n\
Import private read_file_into_list { filename } {\n\
if [$self is_http_url $filename] {\n\
set buffer [$self read_url $filename]\n\
set lines [split $buffer \"\\n\"]\n\
} else {\n\
set f [open $filename \"r\"]\n\
set lines {}\n\
while 1 {\n\
set line [gets $f]\n\
if [eof $f] {\n\
close $f\n\
break\n\
}\n\
lappend lines \"$line\"\n\
}\n\
}\n\
\n\
return $lines\n\
}\n\
\n\
\n\
Import private file_readable { args } {\n\
if { [llength $args] == 0 } {\n\
error \"wrong # args: should be \\\"$self file\\\n\
readable name ?arg ...?\\\"\"\n\
}\n\
\n\
set name [lindex $args 0]\n\
if [$self is_http_url $name] {\n\
$self instvar use_http_cache_ cache_ last_modified_\n\
if $use_http_cache_ {\n\
if { ![info exists cache_] } {\n\
set cache_ [new HTTPCache]\n\
}\n\
\n\
if [info exists last_modified_($name)] {\n\
set buffer [$cache_ get $name \\\n\
$last_modified_($name)]\n\
} else {\n\
set buffer [$cache_ get $name]\n\
}\n\
if { $buffer!={} } {\n\
$self instvar buf_cnt_\n\
if ![info exists buf_cnt_] {\n\
set buf_cnt_ 0\n\
}\n\
set token ::http::readable_$buf_cnt_\n\
upvar #0 $token state\n\
set state(body) $buffer\n\
incr buf_cnt_\n\
return [list 1 $token]\n\
}\n\
}\n\
if [catch {set token [Http geturl $name]} m] {\n\
return [list 0 \"error executing \\\"::http::geturl\\\n\
$name\\\": $m\"]\n\
} else {\n\
set code [::http::code $token]\n\
if {[lindex $code 1] != 200} {\n\
return [list 0 $code]\n\
} else {\n\
if $use_http_cache_ {\n\
if { ![info exists cache_] } {\n\
set cache_ [new HTTPCache]\n\
}\n\
$cache_ put $name [::http::data $token]\n\
}\n\
\n\
return [list 1 $token]\n\
}\n\
}\n\
} else {\n\
eval file readable $args\n\
} \n\
}\n\
\n\
\n\
Import public file { option args } {\n\
if { $option == \"readable\" } {\n\
eval [list $self] file_readable $args\n\
} elseif { $option == \"dirname\" } {\n\
if { [llength $args] == 0 } {\n\
error \"wrong # args: should be \\\"$self file\\\n\
dirname name ?arg ...?\\\"\"\n\
} else {\n\
set name [lindex $args 0]\n\
if [$self is_http_url $name] {\n\
set url $name\n\
regexp {([^:]+)://([^:/]+)(:([0-9]+))?(/.*)} \\\n\
$name url protocol server x \\\n\
port trailingpath\n\
if {[string length $trailingpath] == 0} {\n\
set trailingpath /\n\
}\n\
set trailingpath [file dirname \"$trailingpath\"]\n\
return \"$protocol://$server$x$trailingpath\"\n\
} else {\n\
eval {file $option} $args\n\
}\n\
}\n\
} elseif { $option == \"join\" } {\n\
if { [llength $args] == 0 } {\n\
error \"wrong # args: should be \\\"$self file\\\n\
join name ?arg ...?\\\"\"\n\
} else {\n\
set base_url \"[string trimright [lindex $args 0] /]/\"\n\
set file_name [lindex $args 1]\n\
if [$self is_http_url $file_name] {\n\
return $file_name\n\
}\n\
if { [$self is_http_url $base_url] && \\\n\
[llength $args] ==2 } {\n\
regexp {([^:]+)://([^:/]+)(:([0-9]+))?(/.*)} \\\n\
$base_url url protocol server \\\n\
x port trailingpath\n\
regsub -all {^\\./} $file_name {} file_name\n\
regsub -all {/\\./} $file_name {/} file_name\n\
set counter 0\n\
while [regsub {^\\.\\./} $file_name {} \\\n\
file_name] {\n\
incr counter\n\
}\n\
while { $counter > 0 } {\n\
set trailingpath [$self \\\n\
format_as_dir_string \\\n\
[$self file dirname \\\n\
$trailingpath]]\n\
incr counter -1\n\
}\n\
set trailingpath \"[$self format_as_dir_string \\\n\
$trailingpath]$file_name\"\n\
return \"$protocol://$server$x$trailingpath\"\n\
} else {\n\
eval {file $option} $args\n\
}\n\
}\n\
} else {\n\
eval {file $option} $args\n\
}\n\
}\n\
\n\
\n\
Import private format_as_dir_string { dir_string } {\n\
return \"[string trimright [$self file join $dir_string .] .]\"\n\
}\n\
\n\
\n\
rename source source.orig\n\
proc source {fileName} {\n\
Import instvar instance_\n\
\n\
if ![info exists instance_] {\n\
set instance_ [new Import]\n\
}\n\
\n\
if [$instance_ is_http_url $fileName] {\n\
set buffer [$instance_ read_url $fileName]\n\
uplevel eval $buffer\n\
} else {\n\
uplevel source.orig [list $fileName]\n\
}\n\
}\n\
\n\
\n\
\n\
\n\
proc import args {\n\
if { ![catch \"Import set autoimport_\"] && ![Import set autoimport_] } {\n\
if [catch \"really_import $args\" error_msg] {\n\
error $error_msg\n\
}\n\
}\n\
}\n\
\n\
\n\
proc import args {\n\
Import instvar instance_\n\
\n\
if ![info exists instance_] {\n\
set instance_ [new Import]\n\
}\n\
\n\
if [catch \"eval $instance_ import $args\" errormsg] {\n\
error $errormsg\n\
}\n\
}\n\
\n\
\n\
\n\
proc override_importTable_mapping { object file_list } {\n\
Import instvar instance_\n\
\n\
if ![info exists instance_] {\n\
set instance_ [new Import]\n\
}\n\
\n\
$instance_ override_importTable_mapping $object $file_list\n\
}\n\
\n\
\n\
proc import_use_http_cache { {yes 1} } {\n\
Import instvar instance_\n\
\n\
if ![info exists instance_] {\n\
set instance_ [new Import]\n\
}\n\
\n\
$instance_ set use_http_cache_ 1\n\
}\n\
\n\
\n\
\n\
\n\
Class HTTP\n\
\n\
HTTP public init { } {\n\
$self next\n\
\n\
if { [lsearch -exact [package names] Tk] != -1 } {\n\
$self set enable_output_ 1\n\
} else {\n\
$self set enable_output_ 0\n\
}\n\
$self set token_count_ 0\n\
}\n\
\n\
\n\
HTTP public geturl { url } {\n\
set token [$self start_fetch $url]\n\
$self wait\n\
return $token\n\
}\n\
\n\
HTTP public geturls { args } {\n\
set tokens [eval \"$self start_fetch $args\"] \n\
$self wait\n\
return $tokens\n\
}\n\
\n\
HTTP public start_fetch { args } {\n\
$self instvar token_count_ urls_\n\
\n\
set urls_ $args\n\
foreach url $args {\n\
lappend tokens [::http::geturl $url \\\n\
-progress  \"$self progress_callback\" \\\n\
-command \"$self fetch_done\"]\n\
incr token_count_\n\
}\n\
\n\
if { [llength $tokens] == 1 } {\n\
return [lindex $tokens 0]\n\
} else {\n\
return $tokens\n\
}\n\
}\n\
\n\
\n\
HTTP public wait { } {\n\
$self tkvar vwait_\n\
if { ![info exists vwait_] } {\n\
$self start_ping_pong\n\
if { ![info exists vwait_] } { vwait [$self tkvarname vwait_] }\n\
$self stop_ping_pong\n\
}\n\
unset vwait_\n\
}\n\
\n\
\n\
HTTP private fetch_done { token } {\n\
$self instvar token_count_\n\
$self tkvar vwait_\n\
incr token_count_ -1\n\
if { $token_count_ <= 0 } {\n\
set vwait_ 1\n\
set token_count_ 0\n\
$self instvar total_bytes_ current_bytes_ per_token_\n\
unset total_bytes_ current_bytes_ per_token_\n\
}\n\
}\n\
\n\
\n\
HTTP private progress_callback { token total_bytes current_bytes } {\n\
$self instvar total_bytes_ current_bytes_ per_token_\n\
if { ![info exists total_bytes_] } {\n\
set total_bytes_ 0\n\
set current_bytes_ 0\n\
}\n\
\n\
if { ![info exists per_token_($token)] } {\n\
set per_token_($token) $current_bytes\n\
incr total_bytes_ $total_bytes\n\
incr current_bytes_ $current_bytes\n\
} else {\n\
set current_bytes_ [expr $current_bytes_ - \\\n\
$per_token_($token) + $current_bytes]\n\
set per_token_($token) $current_bytes\n\
}\n\
\n\
$self instvar urls_\n\
$self print_status \"Fetching $urls_ ... (rcvd $current_bytes_ bytes)\"\n\
}\n\
\n\
\n\
HTTP private build_widget { } {\n\
$self instvar frame_ rect_\n\
if { ![info exists frame_] } {\n\
set cnt 0\n\
while [winfo exists .http_$cnt] { incr $cnt }\n\
set frame_ .http_$cnt\n\
toplevel $frame_\n\
wm withdraw $frame_\n\
wm transient $frame_ .\n\
wm title $frame_ \"HTTP Status\"\n\
set new_toplevel 1\n\
}\n\
\n\
set textheight [font metric http_font -linespace]\n\
label $frame_.label -font http_font -width 100 \\\n\
-justify left -anchor w -text \"\"\n\
canvas $frame_.canvas -height $textheight \\\n\
-width 50 -bd 1 -relief sunken\n\
pack $frame_.canvas -side right\n\
pack $frame_.label -expand 1 -fill x -side left\n\
\n\
set rect_ [$frame_.canvas create rectangle 1 2 10 $textheight \\\n\
-fill blue -outline blue]\n\
$frame_.canvas move $rect_ -1000 0\n\
\n\
if [info exists new_toplevel] {\n\
update idletasks\n\
update\n\
set x [expr [winfo screenwidth $frame_]/2 \\\n\
- [winfo reqwidth $frame_]/2 \\\n\
- [winfo vrootx [winfo parent $frame_]]]\n\
set y [expr [winfo screenheight $frame_]/2 \\\n\
- [winfo reqheight $frame_]/2 \\\n\
- [winfo vrooty [winfo parent $frame_]]]\n\
wm geom $frame_ +$x+$y\n\
}       \n\
}\n\
\n\
\n\
HTTP private start_ping_pong { } {\n\
if { ![$self set enable_output_] } return\n\
\n\
$self instvar frame_ rect_ after_id_ hide_id_ dir_ pos_\n\
if { [lsearch -exact [package names] Tk] != -1 } {\n\
\n\
if { ![info exists frame_] } {\n\
$self build_widget\n\
}\n\
\n\
if { [wm state [winfo toplevel $frame_]] == \"withdrawn\" } {\n\
wm deiconify $frame_\n\
}\n\
\n\
if { ![info exists dir_] } {\n\
set dir_ 2\n\
set pos_ 1\n\
}\n\
\n\
set coords [$frame_.canvas coords $rect_]\n\
set x1 [lindex $coords 0]\n\
set y1 [lindex $coords 1]\n\
set x2 [lindex $coords 2]\n\
set y2 [lindex $coords 3]\n\
\n\
$frame_.canvas coords $rect_ $pos_ $y1 [expr $pos_-$x1+$x2] $y2\n\
} else {\n\
$self set ping_cnt_ 0\n\
puts -nonewline stderr \"Fetching URL \"\n\
}\n\
set after_id_ [after 100 \"$self do_ping_pong\"]\n\
if [info exists hide_id_] { after cancel $hide_id_ }\n\
\n\
$self instvar urls_\n\
$self print_status \"Fetching $urls_ ...\"\n\
}\n\
\n\
\n\
HTTP private stop_ping_pong { } {\n\
if { ![$self set enable_output_] } return\n\
\n\
$self instvar after_id_ hide_id_ frame_ rect_\n\
if [info exists after_id_] {\n\
after cancel $after_id_\n\
unset after_id_\n\
if { [lsearch -exact [package names] Tk] != -1 } {\n\
set hide_id_ [after idle \"$self hide\"]\n\
} else {\n\
puts stderr \"\"\n\
}\n\
}\n\
}\n\
\n\
\n\
HTTP private hide { } {\n\
$self instvar frame_ rect_ pos_ dir_\n\
\n\
set pos_ 1\n\
set dir_ 2\n\
\n\
$self print_status \"\"\n\
$frame_.canvas move $rect_ -1000 0\n\
if { [winfo toplevel $frame_] == $frame_ } {\n\
wm withdraw $frame_\n\
}\n\
}\n\
\n\
\n\
HTTP private do_ping_pong { } {\n\
if { ![$self set enable_output_] } return\n\
\n\
$self instvar frame_ rect_ dir_ pos_ after_id_\n\
if { [lsearch -exact [package names] Tk] != -1 } {      \n\
incr pos_ $dir_\n\
$frame_.canvas move $rect_ $dir_ 0\n\
if { $pos_ <= 1 || $pos_ >= 42 } {\n\
set dir_ [expr 0 - $dir_]\n\
}\n\
} else {\n\
$self instvar ping_cnt_\n\
incr ping_cnt_\n\
if { $ping_cnt_ >= 10 } {\n\
puts -nonewline stderr \".\"\n\
set ping_cnt_ 0\n\
}\n\
}\n\
set after_id_ [after 100 \"$self do_ping_pong\"]\n\
}\n\
\n\
\n\
HTTP private print_status { status } {\n\
if { ![$self set enable_output_] } return\n\
\n\
if { [lsearch -exact [package names] Tk] != -1 } {\n\
$self instvar frame_\n\
if [info exists frame_] {\n\
$frame_.label configure -text $status\n\
}\n\
}\n\
}\n\
\n\
\n\
HTTP public set_frame { frame } {\n\
if { [lsearch -exact [package names] Tk] != -1 } {\n\
$self instvar frame_\n\
if [info exists frame_] {\n\
destroy $frame_\n\
}\n\
set frame_ $frame\n\
$self build_widget\n\
}\n\
}\n\
\n\
\n\
HTTP public enable_output { { yes 1 } } {\n\
$self set enable_output_ $yes\n\
}\n\
\n\
\n\
HTTP proc.invoke { } {\n\
if { [lsearch -exact [package names] Tk] != -1 } {\n\
font create http_font -family helvetica -size 10\n\
}\n\
}\n\
\n\
\n\
Class HTTPCache\n\
\n\
\n\
HTTPCache public init { {dir ~/.mash/cache/} } {\n\
$self next\n\
\n\
$self instvar dir_ index_ index_filename_\n\
$self create_dir $dir\n\
set dir_ [glob $dir]\n\
\n\
set index_filename_ [file join $dir_ index.db]\n\
if {! [catch {set f [open $index_filename_]}] } {\n\
while 1 {\n\
set line [gets $f]\n\
if [eof $f] {\n\
close $f\n\
break\n\
}\n\
set index_([lindex $line 0]) [lindex $line 1]\n\
}\n\
}\n\
}\n\
\n\
\n\
HTTPCache public get { url {last_modified {}} } {\n\
$self instvar index_\n\
if [info exists index_($url)] {\n\
if { $last_modified != {} } {\n\
if [catch {set mtime [file mtime $index_($url)]}] \\\n\
{ return \"\" }\n\
if { $last_modified==-1 || $mtime < $last_modified } \\\n\
{ return \"\" }\n\
}\n\
if [catch {set f [open $index_($url)]}] { return \"\" }\n\
fconfigure $f -translation binary\n\
set buffer \"\"\n\
while { ![eof $f] } {\n\
append buffer [read $f 4096]\n\
}\n\
close $f\n\
return $buffer\n\
} else {\n\
return \"\"\n\
}\n\
}\n\
\n\
\n\
HTTPCache public put { url buffer } {\n\
$self instvar index_ dir_ index_filename_\n\
if { ![info exists index_($url)] } {\n\
set update_index_file 1\n\
}\n\
\n\
set name cache[clock clicks]\n\
set index_($url) [file join $dir_ $name[file extension $url]]\n\
\n\
set f [open $index_($url) w 0644]\n\
\n\
fconfigure $f -translation binary\n\
puts -nonewline $f $buffer\n\
close $f\n\
\n\
if [catch {set f [open $index_filename_ a]}] {\n\
set f [open $index_filename_ w 0644]\n\
}\n\
\n\
puts $f [list $url $index_($url)]\n\
close $f\n\
}\n\
\n\
\n\
HTTPCache public flush { } {\n\
$self instvar index_ dir_\n\
file delete -force -- [glob -nocomplain [file join $dir_ *]]\n\
catch {unset index_}\n\
}\n\
\n\
\n\
HTTPCache private create_dir { path } {\n\
if { ![file isdirectory $path] } {\n\
set dir \"\"\n\
foreach split [file split $path] {\n\
set dir [file join $dir $split]\n\
if { ![file exists $dir] } {\n\
file mkdir $dir\n\
}\n\
}\n\
}\n\
}\n\
\n\
\n\
HTTP Http\n\
\n\
\n\
";
#include "tclcl.h"
EmbeddedTcl et_tclobject(code);
