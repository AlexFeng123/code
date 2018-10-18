#!/usr/bin/expect  
set scphost "[lindex $argv 0]" 
set port "[lindex $argv 1]" 
set scpuser "[lindex $argv 2]" 
set scppw "[lindex $argv 3]" 
#要执行的shell命令  
set cmd "[lindex $argv 4]" 
spawn ssh -p $port -t  $scpuser@$scphost "$cmd" 
set timeout 600 
expect {
#the first connect will respose "Are you sure you want to continue connecting (yes/no)? yes" 
"*yes*" {  
send "yes\r" 
}
busy {send_user "\n<error:busy>";exit 1;}  
failed {send_user "\n<error:failed>";exit 2;}  
timeout {send_user "\n<error:timeout>";exit 3;}  
}  
 #Permission denied not try again  
 expect {  
 "*denied*" {  
 send_user "\n<error:Permission denied>" 
 exit 4  
 }  
 busy {send_user "\n<error:busy>";exit 5;}  
 failed {send_user "\n<error:failed>";exit 6;}  
 timeout {send_user "\n<error:timeout>";exit 7;}  
 }  
 exit 0 
