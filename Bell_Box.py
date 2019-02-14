#Ian Bell

#approximate surrounding box in 3n/2 comparison
import random
import math

class Node:
	def __init__(self,xcoor,ycoor,name):
		self.x=xcoor
		self.y=ycoor
		self.num=name #gives the point a reference name for the write up

	def Print(self):
		print "Point %d is located at (%d,%d)" %(self.num,self.x,self.y)

def makePoints(n,size): #n = number of points, size of graph they can be placed on
	pointlist=[]
	for i in range(n):
		pointlist.append(Node(random.randint(-size,size),random.randint(-size,size),i))
		pointlist[i].Print()
	return pointlist

def MinMax(array,l,h): #returns(min,max) l & h are index counters
	#one element
	if l == h:
		MIN = array[l]
		MAX = MIN
		return MIN,MAX
	#two elements
	elif(h == l + 1):
		if (array[l]>array[h]):
			MIN,MAX = array[h],array[l]
		else:
			MIN,MAX = array[l],array[h]
		return MIN,MAX

	#more then two elements
	#break the array in two halves
	mid=int(((l+h)/2))
	minL,maxL = MinMax(array,l,mid)
	minR,maxR = MinMax(array,mid+1,h)

	#check min of each half
	if minL < minR:
		MIN = minL
	else:
		MIN = minR

	#check max of each half
	if maxR > maxL:
		MAX = maxR
	else:
		MAX = maxL

	return MIN,MAX



def DandCbox(S): #set of n points
	xlist=[]
	ylist=[]
	size = 0
	#break into x and y components
	for i in S:
		xlist.append(i.x)
		ylist.append(i.y)
	(left,right)=MinMax(xlist,0,len(S)-1)
	(top,bottom)=MinMax(ylist,0,len(S)-1)
	#print dimensions of R
	print "demensions of R: "
	print "Left side is at x = %d" % (left)
	print "Right side is at x = %d" % (right)
	print "Top side is at y = %d" % (top)
	print "Bottom side is at y = %d" % (bottom)



def main():
	points=makePoints(50,100)#(number of points, range of values for the coordinates)
	DandCbox(points)


if __name__ == '__main__':
	main()
