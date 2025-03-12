# PORT SCANNER :desktop_computer:

A port scanner is a software tool used to scan a network or a host for open ports. Ports are communication endpoints that allow different services and applications to connect and communicate over a network. Port scanners can be used for both legitimate purposes, such as network security assessments and troubleshooting, as well as for malicious activities, like identifying vulnerable systems for potential exploitation.

⚠️ In this following program, I have used threads to make the scanning of ports fast. As using of more number of threads can cause in reduction of performance of the system. So kindly use less number of threads to run the program efficiently.
### **INSTALLATION AND USAGE**
      git clone https://github.com/Ashoksanaka/C-projects.git
      
      cd C-projects/Portscanner
      
      gcc portscanner.c -o "your output file name"

      ./"your output file name" <IP address> <Starting port> <Ending port>
      
##### IF YOU ONLY HAVE DOMAIN NAME:
      ping "Domain Name"
           //or//
      gcc Domain_2_IP.c -o "Your output file name"
      ./"Your output file name"
Run the above commands to get IP address from a domain name
