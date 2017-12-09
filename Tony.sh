#!/bin/bash
passwd='yaoyg'
rm -rf /home/ubuntu/darknet/arguments_file.txt
rm -rf /home/ubuntu/Pictures/prepared/*
rm -rf /home/ubuntu/Pictures/predictions/*
./opencv
./get_file_list.py
./runtestv2.sh
/usr/bin/expect <<-EOF
#set time 30
spawn scp -r /home/ubuntu/Pictures/predictions/. yaoyg@166.111.131.69:~/text
expect {
"(yes/no)?"
 { 
   send "yes\n" 
   expect "*password:" { send "$passwd\n" }
 }
"*password:" { send "$passwd\r" }
}
expect eof
EOF
