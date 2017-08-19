import socket
import codecs
import binascii
from tkinter import *
from tkinter import ttk
import os
import pandas as pd
import numpy as np
import time
from tkinter import messagebox
from tkinter import filedialog
#######################
root = Tk()


root.configure(background = "black")
editor = Frame(root)
editor.configure(background = "black")

loader = Frame(root)
loader.configure(background = "black")

mainmenu = Frame(root)
mainmenu.configure(background = "black")



for frame in (editor, loader, mainmenu):
    frame.grid(row=0, column=0, sticky='news')


root.wm_title("TKK -TRESENJE")
root.config(cursor="none")
root.attributes("-fullscreen",True)
#######################
HOST = "192.168.1.71"            
PORT_502 = 502                                

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.settimeout(5)



tid_counter = 0
data=[]

############# PODATKI IZ TABELE V V VMESNIKU GREJO V DRIVE##############
def val_to_drive(aonearr, atwoarr, velarr, decarr,reparr):

	save_send()
	clear_table()

	aone1 = str(gui_aone1.get())
	aone2 = str(gui_aone2.get())
	aone3 = str(gui_aone3.get())
	aone4 = str(gui_aone4.get())
	aone5 = str(gui_aone5.get())
	aone6 = str(gui_aone6.get())
	aone7 = str(gui_aone7.get())
	aone8 = str(gui_aone8.get())
	aone9 = str(gui_aone9.get())
	aone10 = str(gui_aone10.get())

	atwo1 = str(gui_atwo1.get())
	atwo2 = str(gui_atwo2.get())
	atwo3 = str(gui_atwo3.get())
	atwo4 = str(gui_atwo4.get())
	atwo5 = str(gui_atwo5.get())
	atwo6 = str(gui_atwo6.get())
	atwo7 = str(gui_atwo7.get())
	atwo8 = str(gui_atwo8.get())
	atwo9 = str(gui_atwo9.get())
	atwo10 = str(gui_atwo10.get())

	vel1 = str(gui_vel1.get())
	vel2 = str(gui_vel2.get())
	vel3 = str(gui_vel3.get())
	vel4 = str(gui_vel4.get())
	vel5 = str(gui_vel5.get())
	vel6 = str(gui_vel6.get())
	vel7 = str(gui_vel7.get())
	vel8 = str(gui_vel8.get())
	vel9 = str(gui_vel9.get())
	vel10 = str(gui_vel10.get())

	acc1 = str(gui_acc1.get())
	acc2 = str(gui_acc2.get())
	acc3 = str(gui_acc3.get())
	acc4 = str(gui_acc4.get())
	acc5 = str(gui_acc5.get())
	acc6 = str(gui_acc6.get())
	acc7 = str(gui_acc7.get())
	acc8 = str(gui_acc8.get())
	acc9 = str(gui_acc9.get())
	acc10 = str(gui_acc10.get())

	dec1 = str(gui_dec1.get())
	dec2 = str(gui_dec2.get())
	dec3 = str(gui_dec3.get())
	dec4 = str(gui_dec4.get())
	dec5 = str(gui_dec5.get())
	dec6 = str(gui_dec6.get())
	dec7 = str(gui_dec7.get())
	dec8 = str(gui_dec8.get())
	dec9 = str(gui_dec9.get())
	dec10 = str(gui_dec10.get())

	del1 = str(gui_del1.get())
	del2 = str(gui_del2.get())
	del3 = str(gui_del3.get())
	del4 = str(gui_del4.get())
	del5 = str(gui_del5.get())
	del6 = str(gui_del6.get())
	del7 = str(gui_del7.get())
	del8 = str(gui_del8.get())
	del9 = str(gui_del9.get())
	del10 = str(gui_del10.get())

	rep1 = str(gui_rep1.get())
	rep2 = str(gui_rep2.get())
	rep3 = str(gui_rep3.get())
	rep4 = str(gui_rep4.get())
	rep5 = str(gui_rep5.get())
	rep6 = str(gui_rep6.get())
	rep7 = str(gui_rep7.get())
	rep8 = str(gui_rep8.get())
	rep9 = str(gui_rep9.get())
	rep10 = str(gui_rep10.get())

	aonearr = [aone1, aone2, aone3, aone4, aone5, aone6, aone7, aone8, aone9, aone10]
	atwoarr = [atwo1, atwo2, atwo3, atwo4, atwo5, atwo6, atwo7, atwo8, atwo9, atwo10]
	velarr = [vel1, vel2, vel3, vel4, vel5, vel6, vel7, vel8, vel9, vel10]
	accarr = [acc1, acc2, acc3, acc4, acc5, acc6, acc7, acc8, acc9, acc10]
	decarr = [dec1, dec2, dec3, dec4, dec5, dec6, dec7, dec8, dec9, dec10]
	delarr = [del1, del2, del3, del4, del5, del6, del7, del8, del9, del10]
	reparr = [rep1, rep2, rep3, rep4, rep5, rep6, rep7, rep8, rep9, rep10]

	####### FIXED VALUES ################
	global tid_counter
	count = 0
	protocid = format(0, '04x')
	length = format(47, '04x') 
	slaveid = format(0, '02x') 
	funcode = format(16, '02x') 
	startaddress = format(8192, '04x') 
	regquant = format(20, '04x') 
	bytecount = format(40, '02x') 	
	
	cntl = 16
	cntl = format(cntl, "08x")
	######################################

	for i in range(0, 10):
		print("printing i:", i)
		
		rep_ind = int(reparr[i])
		
		for j in range(0, rep_ind):	

			###################### REQUEST 1 ###############################################

			tid_counter = tid_counter + 1
			transid = format(tid_counter, '04x' )	
			angleone = format((int(aonearr[i])) *51000  , '08x')		
			
			vel = format(((int(velarr[i]))*30)*1000, '08x')
			acc = format((int(accarr[i]))*1000, '08x')
			dec = format((int(decarr[i]))*1000, '08x')
			dly = format(int(delarr[i]), '08x')
			mtnext = format(count + 1, '08x')
			mtset = format(1, '08x')
			drvsav = format(1, '08x')
			counter = format(count, '08x')
			
			if int(delarr[i]) > 0:
				cntl = 48
				cntl = format(cntl, "08x")
			elif int(delarr[i]) == 0:
				cntl = 16
				cntl = format(cntl, "08x")
			values1 = counter + angleone + vel + cntl + acc + dec + mtnext + dly + mtset + drvsav
			request1 = transid + protocid + length + slaveid + funcode + startaddress + regquant + bytecount + values1
			#angleone = bytearray.fromhex(angleone)
			request1 = bytearray.fromhex(request1)
			
			print (angleone)

			print ("transid: ", transid)
			print ("protocid: ", protocid)
			print ("length: ", length)
			print ("slaveid: ", slaveid)
			print ("funcode: ", funcode)
			print ("startaddress: ", startaddress)
			print ("regquant: ", regquant)
			print ("bytecount: ", bytecount)
			print ("counter: ", counter)
			print ("angleone: ", angleone)
			print ("vel: ", vel)
			print ("cntl: ", cntl)
			print ("acc: ", acc)
			print ("dec: ", dec)
			print ("mtnext: ", mtnext)
			print ("dly: ", dly)
			print ("mtset: ", mtset)
			print ("drvsav: ", drvsav)

			s.send(request1)
			print("###### SENDING REQUEST 1#########")
			print("###### STEP ", int(count)+1, " #########")
			print("###### Values sent: ", int(aonearr[i]),int(velarr[i]),int(accarr[i]), int(decarr[i]), int(delarr[i]),  cntl, "########")
			print("###### i value: ", i, " j value: ", j, "##########")
			msg = s.recv(1024)
			count = count + 1
			
			####################### REQUEST 2 ###############################################
			
			tid_counter = tid_counter + 1
			transid = format(tid_counter, '04x' )
			angletwo = format((int(atwoarr[i])) * 51000, '08x')
			vel = format(((int(velarr[i]))*30)*1000, '08x')
			acc = format((int(accarr[i]))*1000, '08x')
			dec = format((int(decarr[i]))*1000, '08x')
			dly = format(int(delarr[i]), '08x')
			mtnext = format(count + 1, '08x')
			counter = format(count, '08x')
			
			if int(delarr[i]) > 0:
				cntl = 48
				cntl = format(cntl, "08x")
			elif int(delarr[i]) == 0:
				cntl = 16
				cntl = format(cntl, "08x")
				

			values2 = counter + angletwo + vel + cntl + acc + dec + mtnext + dly + mtset + drvsav
			request2 = transid + protocid + length + slaveid + funcode + startaddress + regquant + bytecount + values2
			request2 = bytearray.fromhex(request2)
			
			print ("transid: ", transid)
			print ("protocid: ", protocid)
			print ("length: ", length)
			print ("slaveid: ", slaveid)
			print ("funcode: ", funcode)
			print ("startaddress: ", startaddress)
			print ("regquant: ", regquant)
			print ("bytecount: ", bytecount)
			print ("counter: ", counter)
			print ("angletwo: ", angletwo)
			print ("vel: ", vel)
			print ("cntl: ", cntl)
			print ("acc: ", acc)
			print ("dec: ", dec)
			print ("mtnext: ", mtnext)
			print ("dly: ", dly)
			print ("mtset: ", mtset)
			print ("drvsav: ", drvsav)
			s.send(request2)
			print("###### SENDING REQUEST 2#########")
			print("###### STEP ", int(count)+1, " #########")
			print("###### Values sent: ", int(atwoarr[i]),int(velarr[i]),int(accarr[i]), int(decarr[i]), int(delarr[i]), cntl, "########")
			print("###### i value: ", i, " j value: ", j, "##########")
			
			msg2 = s.recv(1024)
			count = count + 1
	
	# Home position requst

	tid_counter = tid_counter + 1
	transid = format(tid_counter, '04x' )
	angletwo = format((int(50)) * 1000, '08x')
	vel = format(((int(10))*30)*1000, '08x')
	acc = format((int(100))*1000, '08x')
	dec = format((int(100))*1000, '08x')
	dly = format(int(0), '08x')
	mtnext = format(count + 1, '08x')
	counter = format(count, '08x')
	mtset = format(1, '08x')
	drvsav = format(1, '08x')
	cntl = format(0, "08x")

	valueshome = counter + angletwo + vel + cntl + acc + dec + mtnext + dly + mtset + drvsav
	requesthome = transid + protocid + length + slaveid + funcode + startaddress + regquant + bytecount + valueshome
	requesthome = bytearray.fromhex(requesthome)
	s.send(requesthome)
	msghome = s.recv(1024)
	
	messagebox.showinfo("Info", "Finished sending data to drive")

####### STARTANJE MOTORJA ####### nastavljenih parametrov			
def start_motion():

	global tid_counter
	tid_counter = tid_counter + 1
	transid = format(tid_counter, '04x' )
	protocid = format(0, '04x')
	length = format(15, '04x') 
	slaveid = format(0, '02x') 
	funcode = format(16, '02x') 
	startaddress3 = format(8212, '04x') 
	regquant = format(4, '04x') 
	bytecount = format(8, '02x') 	
	mtload = format(1, '08x')
	mtmove = format(0, '08x')
	
	values3 = mtload + mtmove
	request3 = transid + protocid + length + slaveid + funcode + startaddress3 + regquant + bytecount + values3
	request3 = bytearray.fromhex(request3)


	try:
		s.send(request3)
	except:
		messagebox.showinfo("No Connection to Drive", "Not Connected to Drive!")
	print("###### STARTING MOTION #########")
	msg3 = s.recv(4096)



########### SHRANJEVANJE V NUMPY TABELO #####
def val_to_np(savefilepath):
	
	aone1 = str(gui_aone1.get())
	aone2 = str(gui_aone2.get())
	aone3 = str(gui_aone3.get())
	aone4 = str(gui_aone4.get())
	aone5 = str(gui_aone5.get())
	aone6 = str(gui_aone6.get())
	aone7 = str(gui_aone7.get())
	aone8 = str(gui_aone8.get())
	aone9 = str(gui_aone9.get())
	aone10 = str(gui_aone10.get())

	atwo1 = str(gui_atwo1.get())
	atwo2 = str(gui_atwo2.get())
	atwo3 = str(gui_atwo3.get())
	atwo4 = str(gui_atwo4.get())
	atwo5 = str(gui_atwo5.get())
	atwo6 = str(gui_atwo6.get())
	atwo7 = str(gui_atwo7.get())
	atwo8 = str(gui_atwo8.get())
	atwo9 = str(gui_atwo9.get())
	atwo10 = str(gui_atwo10.get())

	vel1 = str(gui_vel1.get())
	vel2 = str(gui_vel2.get())
	vel3 = str(gui_vel3.get())
	vel4 = str(gui_vel4.get())
	vel5 = str(gui_vel5.get())
	vel6 = str(gui_vel6.get())
	vel7 = str(gui_vel7.get())
	vel8 = str(gui_vel8.get())
	vel9 = str(gui_vel9.get())
	vel10 = str(gui_vel10.get())

	acc1 = str(gui_acc1.get())
	acc2 = str(gui_acc2.get())
	acc3 = str(gui_acc3.get())
	acc4 = str(gui_acc4.get())
	acc5 = str(gui_acc5.get())
	acc6 = str(gui_acc6.get())
	acc7 = str(gui_acc7.get())
	acc8 = str(gui_acc8.get())
	acc9 = str(gui_acc9.get())
	acc10 = str(gui_acc10.get())

	dec1 = str(gui_dec1.get())
	dec2 = str(gui_dec2.get())
	dec3 = str(gui_dec3.get())
	dec4 = str(gui_dec4.get())
	dec5 = str(gui_dec5.get())
	dec6 = str(gui_dec6.get())
	dec7 = str(gui_dec7.get())
	dec8 = str(gui_dec8.get())
	dec9 = str(gui_dec9.get())
	dec10 = str(gui_dec10.get())

	del1 = str(gui_del1.get())
	del2 = str(gui_del2.get())
	del3 = str(gui_del3.get())
	del4 = str(gui_del4.get())
	del5 = str(gui_del5.get())
	del6 = str(gui_del6.get())
	del7 = str(gui_del7.get())
	del8 = str(gui_del8.get())
	del9 = str(gui_del9.get())
	del10 = str(gui_del10.get())

	rep1 = str(gui_rep1.get())
	rep2 = str(gui_rep2.get())
	rep3 = str(gui_rep3.get())
	rep4 = str(gui_rep4.get())
	rep5 = str(gui_rep5.get())
	rep6 = str(gui_rep6.get())
	rep7 = str(gui_rep7.get())
	rep8 = str(gui_rep8.get())
	rep9 = str(gui_rep9.get())
	rep10 = str(gui_rep10.get())

	aonearr = [aone1, aone2, aone3, aone4, aone5, aone6, aone7, aone8, aone9, aone10]
	atwoarr = [atwo1, atwo2, atwo3, atwo4, atwo5, atwo6, atwo7, atwo8, atwo9, atwo10]
	velarr = [vel1, vel2, vel3, vel4, vel5, vel6, vel7, vel8, vel9, vel10]
	accarr = [acc1, acc2, acc3, acc4, acc5, acc6, acc7, acc8, acc9, acc10]
	decarr = [dec1, dec2, dec3, dec4, dec5, dec6, dec7, dec8, dec9, dec10]
	delarr = [del1, del2, del3, del4, del5, del6, del7, del8, del9, del10]
	reparr = [rep1, rep2, rep3, rep4, rep5, rep6, rep7, rep8, rep9, rep10]

	data = np.array([["1", aone1, atwo1, vel1, acc1, dec1, del1, rep1],
					["2", aone2, atwo2, vel2, acc2, dec2, del2, rep2],	
					["3", aone3, atwo3, vel3, acc3, dec3, del3, rep3],		
					["4", aone4, atwo4, vel4, acc4, dec4, del4, rep4],		
					["5", aone5, atwo5, vel5, acc5, dec5, del5, rep5],
					["6", aone6, atwo6, vel6, acc6, dec6, del6, rep6],
					["7", aone7, atwo7, vel7, acc7, dec7, del7, rep7],
					["8", aone8, atwo8, vel8, acc8, dec8, del8, rep8],	
					["9", aone9, atwo9, vel9, acc9, dec9, del9, rep9],
					["10", aone10, atwo10, vel10, acc10, dec10, del10, rep10]]
				)

	np.save(savefilepath, data)

########### NALAGANJE IZ NP TABELE ##############
def np_to_gui(openfilepath):
	
	dataload = np.load(openfilepath, data)


	gui_aone1.set(dataload[0,1])
	gui_aone2.set(dataload[1,1])
	gui_aone3.set(dataload[2,1])
	gui_aone4.set(dataload[3,1])
	gui_aone5.set(dataload[4,1])
	gui_aone6.set(dataload[5,1])
	gui_aone7.set(dataload[6,1])
	gui_aone8.set(dataload[7,1])
	gui_aone9.set(dataload[8,1])
	gui_aone10.set(dataload[9,1])

	gui_atwo1.set(dataload[0,2])
	gui_atwo2.set(dataload[1,2])
	gui_atwo3.set(dataload[2,2])
	gui_atwo4.set(dataload[3,2])
	gui_atwo5.set(dataload[4,2])
	gui_atwo6.set(dataload[5,2])
	gui_atwo7.set(dataload[6,2])
	gui_atwo8.set(dataload[7,2])
	gui_atwo9.set(dataload[8,2])
	gui_atwo10.set(dataload[9,2])

	gui_vel1.set(dataload[0,3])
	gui_vel2.set(dataload[1,3])
	gui_vel3.set(dataload[2,3])
	gui_vel4.set(dataload[3,3])
	gui_vel5.set(dataload[4,3])
	gui_vel6.set(dataload[5,3])
	gui_vel7.set(dataload[6,3])
	gui_vel8.set(dataload[7,3])
	gui_vel9.set(dataload[8,3])
	gui_vel10.set(dataload[9,3])

	gui_acc1.set(dataload[0,4])
	gui_acc2.set(dataload[1,4])
	gui_acc3.set(dataload[2,4])
	gui_acc4.set(dataload[3,4])
	gui_acc5.set(dataload[4,4])
	gui_acc6.set(dataload[5,4])
	gui_acc7.set(dataload[6,4])
	gui_acc8.set(dataload[7,4])
	gui_acc9.set(dataload[8,4])
	gui_acc10.set(dataload[9,4])

	gui_dec1.set(dataload[0,5])
	gui_dec2.set(dataload[1,5])
	gui_dec3.set(dataload[2,5])
	gui_dec4.set(dataload[3,5])
	gui_dec5.set(dataload[4,5])
	gui_dec6.set(dataload[5,5])
	gui_dec7.set(dataload[6,5])
	gui_dec8.set(dataload[7,5])
	gui_dec9.set(dataload[8,5])
	gui_dec10.set(dataload[9,5])

	gui_del1.set(dataload[0,6])
	gui_del2.set(dataload[1,6])
	gui_del3.set(dataload[2,6])
	gui_del4.set(dataload[3,6])
	gui_del5.set(dataload[4,6])
	gui_del6.set(dataload[5,6])
	gui_del7.set(dataload[6,6])
	gui_del8.set(dataload[7,6])
	gui_del9.set(dataload[8,6])
	gui_del10.set(dataload[9,6])

	gui_rep1.set(dataload[0,7])
	gui_rep2.set(dataload[1,7])
	gui_rep3.set(dataload[2,7])
	gui_rep4.set(dataload[3,7])
	gui_rep5.set(dataload[4,7])
	gui_rep6.set(dataload[5,7])
	gui_rep7.set(dataload[6,7])
	gui_rep8.set(dataload[7,7])
	gui_rep9.set(dataload[8,7])
	gui_rep10.set(dataload[9,7])

###################################
def clearBoard():

	gui_aone1.set(0)
	gui_aone2.set(0)
	gui_aone3.set(0)
	gui_aone4.set(0)
	gui_aone5.set(0)
	gui_aone6.set(0)
	gui_aone7.set(0)
	gui_aone8.set(0)
	gui_aone9.set(0)
	gui_aone10.set(0)

	gui_atwo1.set(0)
	gui_atwo2.set(0)
	gui_atwo3.set(0)
	gui_atwo4.set(0)
	gui_atwo5.set(0)
	gui_atwo6.set(0)
	gui_atwo7.set(0)
	gui_atwo8.set(0)
	gui_atwo9.set(0)
	gui_atwo10.set(0)

	gui_vel1.set(0)
	gui_vel2.set(0)
	gui_vel3.set(0)
	gui_vel4.set(0)
	gui_vel5.set(0)
	gui_vel6.set(0)
	gui_vel7.set(0)
	gui_vel8.set(0)
	gui_vel9.set(0)
	gui_vel10.set(0)

	gui_acc1.set(0)
	gui_acc2.set(0)
	gui_acc3.set(0)
	gui_acc4.set(0)
	gui_acc5.set(0)
	gui_acc6.set(0)
	gui_acc7.set(0)
	gui_acc8.set(0)
	gui_acc9.set(0)
	gui_acc10.set(0)

	gui_dec1.set(0)
	gui_dec2.set(0)
	gui_dec3.set(0)
	gui_dec4.set(0)
	gui_dec5.set(0)
	gui_dec6.set(0)
	gui_dec7.set(0)
	gui_dec8.set(0)
	gui_dec9.set(0)
	gui_dec10.set(0)

	gui_del1.set(0)
	gui_del2.set(0)
	gui_del3.set(0)
	gui_del4.set(0)
	gui_del5.set(0)
	gui_del6.set(0)
	gui_del7.set(0)
	gui_del8.set(0)
	gui_del9.set(0)
	gui_del10.set(0)

	gui_rep1.set(0)
	gui_rep2.set(0)
	gui_rep3.set(0)
	gui_rep4.set(0)
	gui_rep5.set(0)
	gui_rep6.set(0)
	gui_rep7.set(0)
	gui_rep8.set(0)
	gui_rep9.set(0)
	gui_rep10.set(0)

####### POVEZOVANJE Z DRIVEOM######
def connect_to_host():

	try:
		s.connect((HOST, PORT_502))
		
	except:
		nocon = messagebox.showinfo("No Connection!", "Please Connect to Host and try again!")
		
	else:
		pass
		#messagebox.showinfo("Connection Info", "Connection Successful")
		


######### ENABLANJE DRIVEA ###########
def drive_enable():

	global tid_counter
	tid_counter = tid_counter + 1
	transid = format(tid_counter, '04x' )
	protocid = format(0, '04x')
	length = format(11, '04x') 
	slaveid = format(0, '02x') 
	funcode = format(16, '02x') 
	startaddress3 = format(254, '04x') 
	regquant = format(2, '04x') 
	bytecount = format(4, '02x') 	
	drven = format(1, '08x')
	
	
	values4 = drven
	request4 = transid + protocid + length + slaveid + funcode + startaddress3 + regquant + bytecount + values4
	request4 = bytearray.fromhex(request4)


	try:
		s.send(request4)
		msg4 = s.recv(4096)
		print("###### DRIVE ENABLED #########")
	except:
		messagebox.showinfo("No Connection to Drive", "Not Connected to Drive!")
	

	
	

############## BRISANJE PODATKOV IZ TABELE V DRIVEU ################
def clear_table():

	locounter = 0

	for i in range(0, 128):
		global tid_counter
		tid_counter = tid_counter + 1
		transid = format(tid_counter, '04x' )
		protocid = format(0, '04x')
		length = format(11, '04x') 
		slaveid = format(0, '02x') 
		funcode = format(16, '02x') 
		startaddress4 = format(530, '04x') 
		regquant = format(2, '04x') 
		bytecount = format(4, '02x') 	
		mtclear = format(locounter, '08x')
		
	
		values5 = mtclear
		request5 = transid + protocid + length + slaveid + funcode + startaddress4 + regquant + bytecount + values5
		request5 = bytearray.fromhex(request5)


		s.send(request5)
		print("###### TABLE CLEARED #########")
		msg5 = s.recv(4096)
		locounter = locounter + 1


################# SHRANJEVANJE IN NALAGANJE PODATKOV #####################
def open_file():
	openfilepath = "/home/pi/RECIPE/" + rbvar.get() + ".npy"
	#openfilepath = "C:\\Users\\HP - ELITEBOOK\\Desktop\\Recepti\\"+ rbvar.get() + ".npy"
	np_to_gui(openfilepath)

def open_file_loader():
	openfilepath = "/home/pi/RECIPE/" +str( loaderomvar.get()) + ".npy"
	#openfilepath = "C:\\Users\\HP - ELITEBOOK\\Desktop\\Recepti\\"+ loaderomvar.get() + ".npy"
	np_to_gui(openfilepath)

def save_file():
	savefilepath = "/home/pi/RECIPE/"+ rbvar.get() 
	#savefilepath = "C:\\Users\\HP - ELITEBOOK\\Desktop\\Recepti\\"+ rbvar.get()
	val_to_np(savefilepath)
####################################################################################

def save_send():
	savefilepath = "/home/pi/RECIPE/"+ "senddata.npy"
	#savefilepath = "C:\\Users\\HP - ELITEBOOK\\Desktop\\Recepti\\"+ "senddata.npy"
	val_to_np(savefilepath)

def load_send():
	openfilepath = "/home/pi/RECIPE/"+ "senddata.npy"
	#openfilepath = "C:\\Users\\HP - ELITEBOOK\\Desktop\\Recepti\\"+ "senddata.npy"
	np_to_gui(openfilepath)


try:
	connect_to_host()
except Exception as e:
	messagebox.showinfo("Connection Error", "Please Connect to Drive (Check ethernet cable)!")



########## PREMIKANJE MED OKNI #################
def raise_frame(frame):
    frame.tkraise()


def sendloader():
	open_file_loader()
	val_to_drive(aonearr, atwoarr, velarr, decarr,reparr)
########## VARIABLES ########

###################################################################
gui_aone1 = IntVar(root)
gui_aone2 = IntVar(root)
gui_aone3 = IntVar(root)
gui_aone4 = IntVar(root)
gui_aone5 = IntVar(root)
gui_aone6 = IntVar(root)
gui_aone7 = IntVar(root)
gui_aone8 = IntVar(root)
gui_aone9 = IntVar(root)
gui_aone10 = IntVar(root)

gui_atwo1 = IntVar(root)
gui_atwo2 = IntVar(root)
gui_atwo3 = IntVar(root)
gui_atwo4 = IntVar(root)
gui_atwo5 = IntVar(root)
gui_atwo6 = IntVar(root)
gui_atwo7 = IntVar(root)
gui_atwo8 = IntVar(root)
gui_atwo9 = IntVar(root)
gui_atwo10 = IntVar(root)

gui_vel1 = IntVar(root)
gui_vel2 = IntVar(root)
gui_vel3 = IntVar(root)
gui_vel4 = IntVar(root)
gui_vel5 = IntVar(root)
gui_vel6 = IntVar(root)
gui_vel7 = IntVar(root)
gui_vel8 = IntVar(root)
gui_vel9 = IntVar(root)
gui_vel10 = IntVar(root)

gui_acc1 = IntVar(root)
gui_acc2 = IntVar(root)
gui_acc3 = IntVar(root)
gui_acc4 = IntVar(root)
gui_acc5 = IntVar(root)
gui_acc6 = IntVar(root)
gui_acc7 = IntVar(root)
gui_acc8 = IntVar(root)
gui_acc9 = IntVar(root)
gui_acc10 = IntVar(root)

gui_dec1 = IntVar(root)
gui_dec2 = IntVar(root)
gui_dec3 = IntVar(root)
gui_dec4 = IntVar(root)
gui_dec5 = IntVar(root)
gui_dec6 = IntVar(root)
gui_dec7 = IntVar(root)
gui_dec8 = IntVar(root)
gui_dec9 = IntVar(root)
gui_dec10 = IntVar(root)

gui_del1 = IntVar(root)
gui_del2 = IntVar(root)
gui_del3 = IntVar(root)
gui_del4 = IntVar(root)
gui_del5 = IntVar(root)
gui_del6 = IntVar(root)
gui_del7 = IntVar(root)
gui_del8 = IntVar(root)
gui_del9 = IntVar(root)
gui_del10 = IntVar(root)

gui_rep1 = IntVar(root)
gui_rep2 = IntVar(root)
gui_rep3 = IntVar(root)
gui_rep4 = IntVar(root)
gui_rep5 = IntVar(root)
gui_rep6 = IntVar(root)
gui_rep7 = IntVar(root)
gui_rep8 = IntVar(root)
gui_rep9 = IntVar(root)
gui_rep10 = IntVar(root)

###################################################################################
aone1 = str(gui_aone1.get())
aone2 = str(gui_aone2.get())
aone3 = str(gui_aone3.get())
aone4 = str(gui_aone4.get())
aone5 = str(gui_aone5.get())
aone6 = str(gui_aone6.get())
aone7 = str(gui_aone7.get())
aone8 = str(gui_aone8.get())
aone9 = str(gui_aone9.get())
aone10 = str(gui_aone10.get())

atwo1 = str(gui_atwo1.get())
atwo2 = str(gui_atwo2.get())
atwo3 = str(gui_atwo3.get())
atwo4 = str(gui_atwo4.get())
atwo5 = str(gui_atwo5.get())
atwo6 = str(gui_atwo6.get())
atwo7 = str(gui_atwo7.get())
atwo8 = str(gui_atwo8.get())
atwo9 = str(gui_atwo9.get())
atwo10 = str(gui_atwo10.get())

vel1 = str(gui_vel1.get())
vel2 = str(gui_vel2.get())
vel3 = str(gui_vel3.get())	
vel4 = str(gui_vel4.get())
vel5 = str(gui_vel5.get())
vel6 = str(gui_vel6.get())	
vel7 = str(gui_vel7.get())
vel8 = str(gui_vel8.get())
vel9 = str(gui_vel9.get())
vel10 = str(gui_vel10.get())

acc1 = str(gui_acc1.get())
acc2 = str(gui_acc2.get())
acc3 = str(gui_acc3.get())
acc4 = str(gui_acc4.get())
acc5 = str(gui_acc5.get())
acc6 = str(gui_acc6.get())
acc7 = str(gui_acc7.get())
acc8 = str(gui_acc8.get())
acc9 = str(gui_acc9.get())
acc10 = str(gui_acc10.get())

dec1 = str(gui_dec1.get())
dec2 = str(gui_dec2.get())
dec3 = str(gui_dec3.get())
dec4 = str(gui_dec4.get())
dec5 = str(gui_dec5.get())
dec6 = str(gui_dec6.get())
dec7 = str(gui_dec7.get())
dec8 = str(gui_dec8.get())
dec9 = str(gui_dec9.get())
dec10 = str(gui_dec10.get())

del1 = str(gui_del1.get())
del2 = str(gui_del2.get())
del3 = str(gui_del3.get())
del4 = str(gui_del4.get())
del5 = str(gui_del5.get())
del6 = str(gui_del6.get())
del7 = str(gui_del7.get())
del8 = str(gui_del8.get())
del9 = str(gui_del9.get())
del10 = str(gui_del10.get())

rep1 = str(gui_rep1.get())
rep2 = str(gui_rep2.get())
rep3 = str(gui_rep3.get())
rep4 = str(gui_rep4.get())
rep5 = str(gui_rep5.get())
rep6 = str(gui_rep6.get())
rep7 = str(gui_rep7.get())
rep8 = str(gui_rep8.get())
rep9 = str(gui_rep9.get())
rep10 = str(gui_rep10.get())

aonearr = [aone1, aone2, aone3, aone4, aone5, aone6, aone7, aone8, aone9, aone10]
atwoarr = [atwo1, atwo2, atwo3, atwo4, atwo5, atwo6, atwo7, atwo8, atwo9, atwo10]
velarr = [vel1, vel2, vel3, vel4, vel5, vel6, vel7, vel8, vel9, vel10]
accarr = [acc1, acc2, acc3, acc4, acc5, acc6, acc7, acc8, acc9, acc10]
decarr = [dec1, dec2, dec3, dec4, dec5, dec6, dec7, dec8, dec9, dec10]
delarr = [del1, del2, del3, del4, del5, del6, del7, del8, del9, del10]
reparr = [rep1, rep2, rep3, rep4, rep5, rep6, rep7, rep8, rep9, rep10]
###################################################################################
################## OPTION MENU - VALUES ###########################################
valaone = [100, 110, 120, 130, 140, 150, 160, 170]
valatwo = [0, 10, 20, 30, 40, 50, 60, 70, 80, 90] 
valvel = [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130]
valacc = [15000, 16000, 17000, 18000, 19000, 20000]
valdec = [15000, 16000, 17000, 18000, 19000, 20000]
valdel = [0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
valrep = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]

##############################################################################
########################## OPTION MENU #######################################

omfont = ("Helvetica", 22)
omheight = 2
omwidth = 4

omaone1 = OptionMenu(editor, gui_aone1, *valaone)
omaone2 = OptionMenu(editor, gui_aone2, *valaone)
omaone3 = OptionMenu(editor, gui_aone3, *valaone)
omaone4 = OptionMenu(editor, gui_aone4, *valaone)
omaone5 = OptionMenu(editor, gui_aone5, *valaone)
omaone6 = OptionMenu(editor, gui_aone6, *valaone)
omaone7 = OptionMenu(editor, gui_aone7, *valaone)
omaone8 = OptionMenu(editor, gui_aone8, *valaone)
omaone9 = OptionMenu(editor, gui_aone9, *valaone)
omaone10 = OptionMenu(editor, gui_aone10, *valaone)
#####CONFIG####
omaone1.config(height = omheight, width = omwidth, font = omfont,fg ="white", bg = "black")
omaone2.config(height = omheight, width = omwidth, font = omfont,fg ="white", bg = "black")
omaone3.config(height = omheight, width = omwidth, font = omfont,fg ="white", bg = "black")
omaone4.config(height = omheight, width = omwidth, font = omfont,fg ="white", bg ="black")
omaone5.config(height = omheight, width = omwidth, font = omfont,fg ="white", bg = "black")
omaone6.config(height = omheight, width = omwidth, font = omfont,fg = "white", bg = "black")
omaone7.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omaone8.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omaone9.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omaone10.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")

omatwo1 = OptionMenu(editor, gui_atwo1, *valatwo)
omatwo2 = OptionMenu(editor, gui_atwo2, *valatwo)
omatwo3 = OptionMenu(editor, gui_atwo3, *valatwo)
omatwo4 = OptionMenu(editor, gui_atwo4, *valatwo)
omatwo5 = OptionMenu(editor, gui_atwo5, *valatwo)
omatwo6 = OptionMenu(editor, gui_atwo6, *valatwo)
omatwo7 = OptionMenu(editor, gui_atwo7, *valatwo)
omatwo8 = OptionMenu(editor, gui_atwo8, *valatwo)
omatwo9 = OptionMenu(editor, gui_atwo9, *valatwo)
omatwo10 = OptionMenu(editor, gui_atwo10, *valatwo)
#####CONFIG####
omatwo1.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omatwo2.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omatwo3.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omatwo4.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omatwo5.config(height = omheight, width = omwidth, font = omfont, fg = "white",  bg = "black")
omatwo6.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omatwo7.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omatwo8.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omatwo9.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg ="black")
omatwo10.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg ="black")

omvel1 = OptionMenu(editor, gui_vel1, *valvel)
omvel2 = OptionMenu(editor, gui_vel2, *valvel)
omvel3 = OptionMenu(editor, gui_vel3, *valvel)
omvel4 = OptionMenu(editor, gui_vel4, *valvel)
omvel5 = OptionMenu(editor, gui_vel5, *valvel)
omvel6 = OptionMenu(editor, gui_vel6, *valvel)
omvel7 = OptionMenu(editor, gui_vel7, *valvel)
omvel8 = OptionMenu(editor, gui_vel8, *valvel)
omvel9 = OptionMenu(editor, gui_vel9, *valvel)
omvel10 = OptionMenu(editor, gui_vel10, *valvel)
#####CONFIG####
omvel1.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omvel2.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg ="black")
omvel3.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omvel4.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omvel5.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omvel6.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omvel7.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omvel8.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omvel9.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omvel10.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")

omacc1 = OptionMenu(editor, gui_acc1, *valacc)
omacc2 = OptionMenu(editor, gui_acc2, *valacc)
omacc3 = OptionMenu(editor, gui_acc3, *valacc)
omacc4 = OptionMenu(editor, gui_acc4, *valacc)
omacc5 = OptionMenu(editor, gui_acc5, *valacc)
omacc6 = OptionMenu(editor, gui_acc6, *valacc)
omacc7 = OptionMenu(editor, gui_acc7, *valacc)
omacc8 = OptionMenu(editor, gui_acc8, *valacc)
omacc9 = OptionMenu(editor, gui_acc9, *valacc)
omacc10 = OptionMenu(editor, gui_acc10, *valacc)
#####CONFIG####
omacc1.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omacc2.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omacc3.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omacc4.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black" )
omacc5.config(height = omheight, width = omwidth, font = omfont, fg = "white",  bg = "black")
omacc6.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omacc7.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omacc8.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg = "black")
omacc9.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg = "black")
omacc10.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")

omdec1 = OptionMenu(editor, gui_dec1, *valdec)
omdec2 = OptionMenu(editor, gui_dec2, *valdec)
omdec3 = OptionMenu(editor, gui_dec3, *valdec)
omdec4 = OptionMenu(editor, gui_dec4, *valdec)
omdec5 = OptionMenu(editor, gui_dec5, *valdec)
omdec6 = OptionMenu(editor, gui_dec6, *valdec)
omdec7 = OptionMenu(editor, gui_dec7, *valdec)
omdec8 = OptionMenu(editor, gui_dec8, *valdec)
omdec9 = OptionMenu(editor, gui_dec9, *valdec)
omdec10 = OptionMenu(editor, gui_dec10, *valdec)
#####CONFIG####
omdec1.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omdec2.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg = "black")
omdec3.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omdec4.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg ="black")
omdec5.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg ="black")
omdec6.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omdec7.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omdec8.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omdec9.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omdec10.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")

omdel1 = OptionMenu(editor, gui_del1, *valdel)
omdel2 = OptionMenu(editor, gui_del2, *valdel)
omdel3 = OptionMenu(editor, gui_del3, *valdel)
omdel4 = OptionMenu(editor, gui_del4, *valdel)
omdel5 = OptionMenu(editor, gui_del5, *valdel)
omdel6 = OptionMenu(editor, gui_del6, *valdel)
omdel7 = OptionMenu(editor, gui_del7, *valdel)
omdel8 = OptionMenu(editor, gui_del8, *valdel)
omdel9 = OptionMenu(editor, gui_del9, *valdel)
omdel10 = OptionMenu(editor, gui_del10, *valdel)
#####CONFIG####
omdel1.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg = "black")
omdel2.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omdel3.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omdel4.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omdel5.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omdel6.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omdel7.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg = "black")
omdel8.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omdel9.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg = "black")
omdel10.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg = "black")

omrep1 = OptionMenu(editor, gui_rep1, *valrep)
omrep2 = OptionMenu(editor, gui_rep2, *valrep)
omrep3 = OptionMenu(editor, gui_rep3, *valrep)
omrep4 = OptionMenu(editor, gui_rep4, *valrep)
omrep5 = OptionMenu(editor, gui_rep5, *valrep)
omrep6 = OptionMenu(editor, gui_rep6, *valrep)
omrep7 = OptionMenu(editor, gui_rep7, *valrep)
omrep8 = OptionMenu(editor, gui_rep8, *valrep)
omrep9 = OptionMenu(editor, gui_rep9, *valrep)
omrep10 = OptionMenu(editor, gui_rep10, *valrep)
#####CONFIG####
omrep1.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg = "black")
omrep2.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omrep3.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg = "black")
omrep4.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg = "black")
omrep5.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg ="black")
omrep6.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg = "black")
omrep7.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg = "black")
omrep8.config(height = omheight, width = omwidth, font = omfont, fg = "white", bg ="black")
omrep9.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg ="black")
omrep10.config(height = omheight, width = omwidth, font = omfont, fg ="white", bg ="black")

##########################################################
######### OPTION MENU - SUB ##############################

subfont = ("Helvetica", 25)

smaone1 = omaone1.nametowidget(omaone1.menuname)
smaone2 = omaone2.nametowidget(omaone2.menuname)
smaone3 = omaone3.nametowidget(omaone3.menuname)
smaone4 = omaone4.nametowidget(omaone4.menuname)
smaone5 = omaone5.nametowidget(omaone5.menuname)
smaone6 = omaone6.nametowidget(omaone6.menuname)
smaone7 = omaone7.nametowidget(omaone7.menuname)
smaone8 = omaone8.nametowidget(omaone8.menuname)
smaone9 = omaone9.nametowidget(omaone9.menuname)
smaone10 = omaone10.nametowidget(omaone10.menuname)
#####CONFIG####
smaone1.config(font = subfont)
smaone2.config(font = subfont)
smaone3.config(font = subfont)
smaone4.config(font = subfont)
smaone5.config(font = subfont)
smaone6.config(font = subfont)
smaone7.config(font = subfont)
smaone8.config(font = subfont)
smaone9.config(font = subfont)
smaone10.config(font = subfont)

smatwo1 = omatwo1.nametowidget(omatwo1.menuname)
smatwo2 = omatwo2.nametowidget(omatwo2.menuname)
smatwo3 = omatwo3.nametowidget(omatwo3.menuname)
smatwo4 = omatwo4.nametowidget(omatwo4.menuname)
smatwo5 = omatwo5.nametowidget(omatwo5.menuname)
smatwo6 = omatwo6.nametowidget(omatwo6.menuname)
smatwo7 = omatwo7.nametowidget(omatwo7.menuname)
smatwo8 = omatwo8.nametowidget(omatwo8.menuname)
smatwo9 = omatwo9.nametowidget(omatwo9.menuname)
smatwo10 = omatwo10.nametowidget(omatwo10.menuname)
#####CONFIG####
smatwo1.config(font = subfont)
smatwo2.config(font = subfont)
smatwo3.config(font = subfont)
smatwo4.config(font = subfont)
smatwo5.config(font = subfont)
smatwo6.config(font = subfont)
smatwo7.config(font = subfont)
smatwo8.config(font = subfont)
smatwo9.config(font = subfont)
smatwo10.config(font = subfont)

smavel1 = omvel1.nametowidget( omvel1.menuname)
smavel2 = omvel2.nametowidget( omvel2.menuname)
smavel3 = omvel3.nametowidget( omvel3.menuname)
smavel4 = omvel4.nametowidget( omvel4.menuname)
smavel5 = omvel5.nametowidget( omvel5.menuname)
smavel6 = omvel6.nametowidget( omvel6.menuname)
smavel7 = omvel7.nametowidget( omvel7.menuname)
smavel8 = omvel8.nametowidget( omvel8.menuname)
smavel9 = omvel9.nametowidget( omvel9.menuname)
smavel10 = omvel10.nametowidget( omvel10.menuname)
#####CONFIG####
smavel1.config(font = subfont)
smavel2.config(font = subfont)
smavel3.config(font = subfont)
smavel4.config(font = subfont)
smavel5.config(font = subfont)
smavel6.config(font = subfont)
smavel7.config(font = subfont)
smavel8.config(font = subfont)
smavel9.config(font = subfont)
smavel10.config(font = subfont)

smacc1 = omacc1.nametowidget(omacc1.menuname)
smacc2 = omacc2.nametowidget(omacc2.menuname)
smacc3 = omacc3.nametowidget(omacc3.menuname)
smacc4 = omacc4.nametowidget(omacc4.menuname)
smacc5 = omacc5.nametowidget(omacc5.menuname)
smacc6 = omacc6.nametowidget(omacc6.menuname)
smacc7 = omacc7.nametowidget(omacc7.menuname)
smacc8 = omacc8.nametowidget(omacc8.menuname)
smacc9 = omacc9.nametowidget(omacc9.menuname)
smacc10 = omacc10.nametowidget(omacc10.menuname)
#####CONFIG####
smacc1.config(font = subfont)
smacc2.config(font = subfont)
smacc3.config(font = subfont)
smacc4.config(font = subfont)
smacc5.config(font = subfont)
smacc6.config(font = subfont)
smacc7.config(font = subfont)
smacc8.config(font = subfont)
smacc9.config(font = subfont)
smacc10.config(font = subfont)

smadec1 = omdec1.nametowidget(omdec1.menuname)
smadec2 = omdec2.nametowidget(omdec2.menuname)
smadec3 = omdec3.nametowidget(omdec3.menuname)
smadec4 = omdec4.nametowidget(omdec4.menuname)
smadec5 = omdec5.nametowidget(omdec5.menuname)
smadec6 = omdec6.nametowidget(omdec6.menuname)
smadec7 = omdec7.nametowidget(omdec7.menuname)
smadec8 = omdec8.nametowidget(omdec8.menuname)
smadec9 = omdec9.nametowidget(omdec9.menuname)
smadec10 = omdec10.nametowidget(omdec10.menuname)
#####CONFIG####
smadec1.config(font = subfont)
smadec2.config(font = subfont)
smadec3.config(font = subfont)
smadec4.config(font = subfont)
smadec5.config(font = subfont)
smadec6.config(font = subfont)
smadec7.config(font = subfont)
smadec8.config(font = subfont)
smadec9.config(font = subfont)
smadec10.config(font = subfont)

smadel1 = omdel1.nametowidget(omdel1.menuname)
smadel2 = omdel2.nametowidget(omdel2.menuname)
smadel3 = omdel3.nametowidget(omdel3.menuname)
smadel4 = omdel4.nametowidget(omdel4.menuname)
smadel5 = omdel5.nametowidget(omdel5.menuname)
smadel6 = omdel6.nametowidget(omdel6.menuname)
smadel7 = omdel7.nametowidget(omdel7.menuname)
smadel8 = omdel8.nametowidget(omdel8.menuname)
smadel9 = omdel9.nametowidget(omdel9.menuname)
smadel10 = omdel10.nametowidget(omdel10.menuname)
#####CONFIG####
smadel1.config(font = subfont)
smadel2.config(font = subfont)
smadel3.config(font = subfont)
smadel4.config(font = subfont)
smadel5.config(font = subfont)
smadel6.config(font = subfont)
smadel7.config(font = subfont)
smadel8.config(font = subfont)
smadel9.config(font = subfont)
smadel10.config(font = subfont)

smarep1 = omrep1.nametowidget(omrep1.menuname)
smarep2 = omrep2.nametowidget(omrep2.menuname)
smarep3 = omrep3.nametowidget(omrep3.menuname)
smarep4 = omrep4.nametowidget(omrep4.menuname)
smarep5 = omrep5.nametowidget(omrep5.menuname)
smarep6 = omrep6.nametowidget(omrep6.menuname)
smarep7 = omrep7.nametowidget(omrep7.menuname)
smarep8 = omrep8.nametowidget(omrep8.menuname)
smarep9 = omrep9.nametowidget(omrep9.menuname)
smarep10 = omrep10.nametowidget(omrep10.menuname)
#####CONFIG####
smarep1.config(font = subfont)
smarep2.config(font = subfont)
smarep3.config(font = subfont)
smarep4.config(font = subfont)
smarep5.config(font = subfont)
smarep6.config(font = subfont)
smarep7.config(font = subfont)
smarep8.config(font = subfont)
smarep9.config(font = subfont)
smarep10.config(font = subfont)

##################################################
####################BUTTONS#######################
##################################################
labelstep1 = Label(editor, text = "1", font = ("Helvetica", 16))
labelstep2 = Label(editor, text = "2", font = ("Helvetica", 16))
labelstep3 = Label(editor, text = "3", font = ("Helvetica", 16))
labelstep4 = Label(editor, text = "4", font = ("Helvetica", 16))
labelstep5 = Label(editor, text = "5", font = ("Helvetica", 16))
labelstep6 = Label(editor, text = "6", font = ("Helvetica", 16))
labelstep7 = Label(editor, text = "7",font = ("Helvetica", 16))
labelstep8 = Label(editor, text = "8", font = ("Helvetica", 16))
labelstep9 = Label(editor, text = "9", font = ("Helvetica", 16))
labelstep10 = Label(editor, text = "10", font = ("Helvetica", 16))

##################################################
labelstep = Label(editor, text = "#",font = ("Helvetica", 16))
labelaone = Label(editor, text = "Angle #1",font = ("Helvetica", 16)) 
labelatwo = Label(editor, text = "Angle #2", font = ("Helvetica", 16)) 
labelvel = Label(editor, text = "Velocity", font = ("Helvetica", 16))
labelacc = Label(editor, text = "Accel.", font = ("Helvetica", 16)) 
labeldec = Label(editor, text = "Decel.", font = ("Helvetica", 16)) 
labeldel = Label(editor, text = "Delay", font = ("Helvetica", 16)) 
labelrep = Label(editor, text = "Execution", font = ("Helvetica", 16))
labelrec = Label(editor, text = "Recipe", font = ("Helvetica", 16)) 
##################################################
labelstep.grid(row = 0, column = 0,padx = (0,10), pady = (0, 20))
labelaone.grid(row = 0, column = 1, pady = (0, 20))
labelatwo.grid(row = 0, column = 2, pady = (0, 20))
labelvel.grid(row = 0, column = 3, pady = (0, 20))
labelacc.grid(row = 0, column = 4, pady = (0, 20))
labeldec.grid(row = 0, column = 5, pady = (0, 20))
labeldel.grid(row = 0, column = 6, pady = (0, 20))
labelrep.grid(row = 0, column = 7, pady = (0, 20))
labelrec.grid(row = 0, column = 8,padx = (20, 0), pady = (0, 20))

labelstep1.grid(row = 1, column = 0,padx = (0,10))
labelstep2.grid(row = 2, column = 0,padx = (0,10))
labelstep3.grid(row = 3, column = 0,padx = (0,10))
labelstep4.grid(row = 4, column = 0,padx = (0,10))
labelstep5.grid(row = 5, column = 0,padx = (0,10))

omaone1.grid(row = 1 , column = 1, padx = (0, 5), pady = (0, 10))
omaone2.grid(row = 2 , column = 1, padx = (0, 5),pady = (0, 10))
omaone3.grid(row = 3 , column = 1, padx = (0, 5),pady = (0, 10))
omaone4.grid(row = 4 , column = 1, padx = (0, 5),pady = (0, 10))
omaone5.grid(row = 5 , column = 1, padx = (0, 5),pady = (0, 10))

omatwo1.grid(row = 1 , column = 2, padx = (0, 5),pady = (0, 10))
omatwo2.grid(row = 2 , column = 2, padx = (0, 5),pady = (0, 10))
omatwo3.grid(row = 3 , column = 2, padx = (0, 5),pady = (0, 10))
omatwo4.grid(row = 4 , column = 2, padx = (0, 5),pady = (0, 10))
omatwo5.grid(row = 5 , column = 2, padx = (0, 5),pady = (0, 10))

omvel1.grid(row = 1 , column = 3, padx = (0, 5),pady = (0, 10))
omvel2.grid(row = 2 , column = 3, padx = (0, 5),pady = (0, 10))
omvel3.grid(row = 3 , column = 3, padx = (0, 5),pady = (0, 10))
omvel4.grid(row = 4 , column = 3, padx = (0, 5),pady = (0, 10))
omvel5.grid(row = 5 , column = 3, padx = (0, 5),pady = (0, 10))


omacc1.grid(row = 1 , column = 4, padx = (0, 5),pady = (0, 10))
omacc2.grid(row = 2 , column = 4, padx = (0, 5),pady = (0, 10))
omacc3.grid(row = 3 , column = 4, padx = (0, 5),pady = (0, 10))
omacc4.grid(row = 4 , column = 4, padx = (0, 5),pady = (0, 10))
omacc5.grid(row = 5 , column = 4, padx = (0, 5),pady = (0, 10))


omdec1.grid(row = 1 , column = 5, padx = (0, 5),pady = (0, 10))
omdec2.grid(row = 2 , column = 5, padx = (0, 5),pady = (0, 10))
omdec3.grid(row = 3 , column = 5, padx = (0, 5),pady = (0, 10))
omdec4.grid(row = 4 , column = 5, padx = (0, 5),pady = (0, 10))
omdec5.grid(row = 5 , column = 5, padx = (0, 5),pady = (0, 10))

omdel1.grid(row = 1 , column = 6, padx = (0, 5),pady = (0, 10))
omdel2.grid(row = 2 , column = 6, padx = (0, 5),pady = (0, 10))
omdel3.grid(row = 3 , column = 6, padx = (0, 5),pady = (0, 10))
omdel4.grid(row = 4 , column = 6, padx = (0, 5),pady = (0, 10))
omdel5.grid(row = 5 , column = 6, padx = (0, 5),pady = (0, 10))


omrep1.grid(row = 1 , column = 7, padx = (0, 5),pady = (0, 10))
omrep2.grid(row = 2 , column = 7, padx = (0, 5),pady = (0, 10))
omrep3.grid(row = 3 , column = 7, padx = (0, 5),pady = (0, 10))
omrep4.grid(row = 4 , column = 7, padx = (0, 5),pady = (0, 10))
omrep5.grid(row = 5 , column = 7, padx = (0, 5),pady = (0, 10))


rbvar = StringVar()
rb1 = Radiobutton(editor, text = "1", variable = rbvar, font = ("Helvetica", 16), value = "Recipe1", command = lambda:open_file())
rb2 = Radiobutton(editor, text = "2", variable = rbvar, font = ("Helvetica", 16), value = "Recipe2", command = lambda:open_file() )
rb3 = Radiobutton(editor, text = "3", variable = rbvar, font = ("Helvetica", 16), value = "Recipe3", command = lambda:open_file())
rb4 = Radiobutton(editor, text = "4", variable = rbvar, font = ("Helvetica", 16), value = "Recipe4", command = lambda:open_file() )
rb5 = Radiobutton(editor, text = "5", variable = rbvar, font = ("Helvetica", 16), value = "Recipe5", command = lambda:open_file() )
rb6 = Radiobutton(editor, text = "6", variable = rbvar, font = ("Helvetica", 16), value = "Recipe6", command = lambda:open_file())
rb7 = Radiobutton(editor, text = "7", variable = rbvar, font = ("Helvetica", 16), value = "Recipe7", command = lambda:open_file() )
rb8 = Radiobutton(editor, text = "8", variable = rbvar, font = ("Helvetica", 16), value = "Recipe8", command = lambda:open_file())
rb9 = Radiobutton(editor, text = "9", variable = rbvar, font = ("Helvetica", 16), value = "Recipe9", command = lambda:open_file() )
rb10 = Radiobutton(editor, text = "10", variable = rbvar, font = ("Helvetica", 16), value = "Recipe10", command = lambda:open_file() )

savebutton = Button(editor, text = "Save", command = lambda: save_file(), font = ("Helvetica", 20))
sendbutton = Button(editor, text = "Send", command = lambda:val_to_drive(aonearr, atwoarr, velarr, decarr,reparr), font = ("Helvetica", 20)) 
startbutton = Button(editor, text = "Start", command = lambda:start_motion(), font = ("Helvetica", 20))
enabledrivebutton = Button(editor, text = "Enable", command = lambda : drive_enable(), font = ("Helvetica", 20))
backbutton = Button(editor, text = "Back", command = lambda : raise_frame(mainmenu), font = ("Helvetica", 20))
clearbutton = Button(editor, text = "Clear", command = lambda: clearBoard(), font = ("Helvetica", 20))

rb1.grid(row = 1 , column = 8,padx = (10, 0),pady = (20, 0))
rb2.grid(row = 2, column = 8,padx = (10, 0),pady = (20, 0))
rb3.grid(row = 3, column = 8,padx = (10, 0),pady = (20, 0))
rb4.grid(row = 4, column = 8,padx = (10, 0),pady = (20, 0))
rb5.grid(row = 5, column = 8,padx = (10, 0),pady = (20, 0))
#rb6.grid(row = 7, column = 1,padx = (0, 5),pady = (20, 0))
#rb7.grid(row = 7, column = 2,padx = (0, 5),pady = (20, 0))
#rb8.grid(row = 7, column = 3,padx = (0, 5),pady = (20, 0))
#rb9.grid(row = 7, column = 4,padx = (0, 5),pady = (20, 0))
#rb10.grid(row = 7, column = 5,padx = (0, 5),pady = (20, 0))

backbutton.grid(row = 6, column = 1, pady = (20, 0))
clearbutton.grid(row = 6, column = 3, pady = (20, 0))

savebutton.grid(row = 6, column = 5, pady = (20, 0))
sendbutton.grid(row = 6, column = 6, pady = (20, 0))
startbutton.grid(row = 6, column = 7, pady = (20, 0))





##################################################
################## MAIN MENU #####################

mainmenulabel = Label(mainmenu, text = "MAINMENU", font = ("Helvetica", 50))
editorbutton = Button(mainmenu, text = "EDITOR", command = lambda: raise_frame(editor), font = ("Helvetica", 30))
loaderbutton = Button(mainmenu, text = "LOADER", command = lambda: raise_frame(loader), font = ("Helvetica", 40))

mainmenulabel.grid(row = 0, column = 0, padx = (330, 0), pady = (100, 0))
editorbutton.grid(row = 2, column = 0, padx = (330, 0), pady = (70, 0))
loaderbutton.grid(row = 1, column = 0, padx = (330, 0), pady = (80, 0))

############ LOADER ##############################

sendbutton = Button(loader, text = "SEND", command = lambda:sendloader(), font = ("Helvetica", 35)) 
sendbutton.grid(row = 2, column = 0, padx = (330, 0), pady = (50, 0) )
backbutton = Button(loader, text = "BACK", command = lambda : raise_frame(mainmenu), font = ("Helvetica", 16))
backbutton.grid(row = 3, column = 0,  padx = (330, 0), pady = (60, 0))

loaderlabel = Label(loader, text = "LOADER", font = ("Helvetica", 40))
loaderlabel.grid(row = 0, column = 0,  padx = (330, 0), pady = (50, 0)) 

recarr = ["Recipe1", "Recipe2", "Recipe3", "Recipe4", "Recipe5", "Recipe6", "Recipe7", "Recipe8","Recipe9", "Recipe10"]
loaderomvar = StringVar()
loaderom = OptionMenu(loader, loaderomvar, *recarr )
loaderomvar.set("PICK A RECIPE")
loaderom.config(height = 5, width = 20,font = ("Helvetica", 20) )
loaderom.grid(row = 1, column = 0,  padx = (330, 0), pady = (50, 0))
sloaderom = loaderom.nametowidget(loaderom.menuname)
sloaderom.config(font = ("Helvetica", 25))
##################################################

load_send()
root.minsize(1024,600);
drive_enable()
##################################################
raise_frame(mainmenu)
root.mainloop()
