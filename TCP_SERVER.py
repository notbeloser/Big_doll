import socket, threading
IP="192.168.0.12"
PORT=6000
BUFFER_SIZE=1024

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind((IP,PORT))
s.listen(1)
Flag = True
def main():
	global Flag
	conn, addr=s.accept()
	print 'Connection address:', addr
	conn.send('A')
	thd1=threading.Thread(target=recv, args=(conn,), name='thread-recv')
	thd1.start()
	while(Flag):
		i=raw_input('send data>>>')
		if(i=='q'or i=='Q'):
			Flag=False
		else:
			#unicode_str=unicode(i,'big5')
			#print unicode_str
			#print unicode_str.__class__
			#s.send(unicode_str.encode('utf-8'))
			conn.send(i)
			#s.send("\n




def recv(s):
	global Flag
	while Flag:
		
		data=s.recv(BUFFER_SIZE)
		if not data: break
		print "received data:", data

	s.close()

	
if __name__=='__main__':
	main()
