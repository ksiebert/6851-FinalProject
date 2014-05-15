import random

class Point:
	def __init__(self, x, y):
		self.x = x
		self.y = y

	def ccw(self, p2, p3):
		return (p3.y-self.y)*(p2.x-self.x) > (p2.y-self.y)*(p3.x-self.x)

	def __eq__(self, p2):
		return self.x == p2.x and self.y == p2.y

class Line:
	# Direction: 1 -> vertical
	# Direction: 0 -> horizontal
	def __init__(self, p1, p2, direction):
		self.p1 = p1
		self.p2 = p2
		self.dir = direction

	def intersect(self, line):
		return self.p1.ccw(line.p1,line.p2) != self.p2.ccw(line.p1,line.p2) and self.p1.ccw(self.p2,line.p1) != self.p1.ccw(self.p2,line.p2)

	def __str__(self):
		return "Line: (" + str(self.p1.x) + ", " + str(self.p1.y) + "), (" + str(self.p2.x) + ", " + str(self.p2.y) + ")"

	def __eq__(self, line2):
		return (self.p1 == line2.p1 and self.p2 == line2.p2) or (self.p2 == line2.p1 and self.p1 == line2.p2)

class Collision:
	def __init__(self, line1, line2):
		self.line1 = line1
		self.line2 = line2

	def __str__(self):
		return "Collision: " + str(self.line1) + ", " + str(self.line2)

	def __eq__(self, collision):
		return (self.line1 == collision.line1 and self.line2 == collision.line2) or (self.line2 == collision.line1 and self.line2 == collision.line2)

# implementation of distribution sweeping algorithm
def distribution_sweep(lines, basecase):
	collisions = []
	print len(lines)
	if len(lines) <= basecase:
		active_lines = []
		y_sorted = sorted(lines, key=lambda line: line.p1.y)
		for line in y_sorted:
			if line.dir > 0: 
				active_lines.append(line)
			else:
				for line2 in active_lines:
					if line.intersect(line2):
						c = Collision(line,line2)
						collisions.append(c)
					else:
						# current active line can collide with no more lines if condition is met
						if line2.p1.x <= line.p2.x and line2.p1.x >= line.p1.x:
							active_lines.remove(line2)
		return collisions
	
	# Recurse
	left_lines = []
	right_lines = []
	median = sum(line.p1.x for line in lines) / len(lines)
	for line in lines: 
		if line.p1.x < median:
			left_lines.append(line)
			if line.p2.x > median:
				right_lines.append(line)
		else:
			right_lines.append(line)

	collisions.extend(distribution_sweep(left_lines, basecase))
	collisions.extend(distribution_sweep(right_lines, basecase))
	return collisions



# create random set of lines
lines = []
max_length = 100
min_length = 5
window = 1000
for i in range(1000):
	length = (int)(random.random() * (max_length - min_length) + min_length)
	direction = (int)(random.random() > .5)
	x0 = (int)(random.random() * window)
	y0 = (int)(random.random() * window)
	if direction:
		y1 = y0 + length
		x1 = x0
	else:
		y1 = y0
		x1 = x0 + length

	p1 = Point(x0,y0)
	p2 = Point(x1,y1)
	line = Line(p1,p2, direction)
	lines.append(line)

collisions = distribution_sweep(lines, 128)
print "Total Collisions: " + str(len(collisions))
for c in collisions:
	print str(c)

# check for duplicates
for i in range(len(collisions)):
	for j in range(i, len(collisions)):
		if i != j:
			if collisions[i] == collisions[j]:
				print "DUPLICATE!!!!!"
				print str(collisions[j])
				print str(collisions[i])






