#!/usr/bin/python
import paramiko  
import psutil  
import sys
linux=['10.190.9.40'] 
def connectHost(ip,uname='root'):
     ssh=paramiko.SSHClient()  
     sh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
     ssh.connect(ip,port=614,username=uname) 
     return ssh

if __name__=='__main__':  
    connectHost(linux[0])
