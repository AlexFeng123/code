#!/usr/bin/expect  
set host "[lindex $argv 0]" 
set file "[lindex $argv 1]"
set dest "[lindex $argv 2]"
set timeout 600
spawn scp -o Port=614 $file root@$host:$dest
expect {
   "(yes/no)?"
   {
       send "yes\n"
   }
}
expect eof
