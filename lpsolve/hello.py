#pip install pulp
import pulp
from pulp.solvers import LpMaximize, LpMinimize
from pulp import LpVariable, LpProblem, LpStatus, value

#create variables with ranges
x = LpVariable("x", 0)
y = LpVariable("y", 0,None, pulp.LpInteger)
#create problem
prob = LpProblem("myProblem", LpMaximize)

#create exprations and constraintd
print "1"
prob += x >= 100
print "2"
prob += x <= 200.4 
print "3"
prob += y >= 80
print "4"
prob += y <= 170.4
print "5"
prob += y >= -x + 200
print "6"
prob += -2*x + 5*y

status = prob.solve()

print LpStatus[status]

print value(x), value(y)
