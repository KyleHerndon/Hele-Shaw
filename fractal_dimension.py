from __future__ import print_function
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import threshold
from scipy.spatial import ConvexHull
import sys as sys
import string as st

def edge_find(image):
  #Edge find takes a bit-valued array (1s & 0s) and returns a modified array
  #with all of the interior elements deleted. Interior elements are elements that,
  #in the cartesian grid, have exactly 4 neighbors.
  dim = np.shape(image)
  m = dim[0]
  n = dim[1]
  #print(m,n)
  image_edge = np.zeros(dim)
  for i in range(1, m-1):
    for j in range(1, n-1):
      if image[i,j]+image[i+1,j]+image[i-1,j]+image[i,j+1]+image[i,j-1] > 4.0:
        image_edge[i,j] = 0.0
      elif image[i,j] == 1.0:
        image_edge[i,j] = 1.0
      #end if
    #end for
  #end for
  return image_edge
#end def edge_find

def convert_to_points(A):
  #Convert to points takes a bit-valued array (1s & 0s) and converts it to a
  #series of x,y pairs assuming the length of the domain in each dimension is
  #1.0.
  dim = np.shape(A)
  m = dim[0]
  n = dim[1]
  x = np.zeros(np.int(np.sum(A)))
  y = np.zeros(np.int(np.sum(A)))
  k = 0
  #Assuming x and y domain sizes of 1, this for loop assigns x and y points to
  #each element in the bit-valued array.
  for i in range(m):
    for j in range(n):
      if A[i,j]==1:
        x[k] = np.float(i) / m
        y[k] = np.float(j) / n
        k += 1
      #end if
    #end for
  #end for
  return x,y
#end def convert_to_points

def count_boxes(x, y, r):
  #count_boxes takes a series of x,y points that represent the locations of
  #random walkers on the edge of the aggregate and, given a bin size r,
  #constructs a histogram of walker positions and counts the number of bins that
  #are occupied by one or more walkers. This value is returned.
  xgrid = np.linspace(0.0, 1.0, 1.0 / r)
  ygrid = np.linspace(0.0, 1.0, 1.0 / r)
  m = np.size(xgrid)
  n = np.size(ygrid)
  filled = np.zeros((m,n))
  #This section bins the occupied boxes and counts the number occupied.
  bins, xedges, yedges = np.histogram2d(x,y,bins=1.0/r)
  bins = threshold(bins, threshmin=0.0, threshmax=0.0, newval=1.0)
  return np.sum(bins)
#end def count_boxes

def main():
  #load the image from a bit-valued text-array
  image = np.loadtxt('out.csv',delimiter=' ')
  f1=open('FD.out','a+')
  f2=open('Convex.out','a+')
  seed = int(sys.argv[1])
  A = float(sys.argv[2])
  B = float(sys.argv[3])
  C = float(sys.argv[4])
  #image = np.abs(image - 1.0)
  #find the matching array with all interior points deleted
  boundary = edge_find(image)
  #calculate the ratio of the area to the area of the convex hull
  indices = np.array(np.nonzero(boundary)).T
  area1 = np.count_nonzero(image)
  hull = ConvexHull(indices)
  area2 = hull.volume
  print(area1/area2,file=f2)
  #convert the edge points to x and y coordinates
  x,y = convert_to_points(boundary)
  plt.figure(1)
  plt.plot(x,y,'.')
  plt.title('Hele-Shaw A='+str(A)+' B='+str(B)+' C='+str(C))
  plt.axis([0.0, 1.0, 0.0, 1.0])
  plt.xlabel('x')
  plt.ylabel('y')
  output_filename = 'HS_A='+str(A)+'_B='+str(B)+'_C='+str(C)+'_'+str(seed)
  output_filename = st.replace(output_filename,'.','p')
  plt.savefig(output_filename+'.png')
  #Using 100 different ruler sizes, starting from a ruler of size 0.1 and going
  #to a ruler of size 0.4, set up the r array
  n = 3
  r0 = 0.01
  r1 = 0.2
  r = np.linspace(r0,r1,n)
  N = np.zeros(n)
  #For every ruler length in the r array, count the number of boxes
  for i in range(n):
    N[i] = count_boxes(x, y, r[i])
  #end for
  #Find the logarithms of both quantities.
  lr = np.log(r)
  ln = np.log(N)
  #Plot the resulting scaling law line.
  plt.figure(2)
  plt.plot(lr,ln)
  plt.title('Fractal Scaling for A=4.0, B=0.5, C=0.0')
  plt.xlabel('log(Box Size)')
  plt.ylabel('log(Number of Occupied Boxes)')
  plt.savefig('Fractal_Scaling.png')
  #Find the slope of the line using linear least squares regression.
  slope = -(np.average(lr*ln) - np.average(lr)*np.average(ln)) / (np.average(lr**2) - np.average(lr)**2)
  print(slope,file=f1)
  print(slope)
#end def main

if __name__ == '__main__':
  main()
