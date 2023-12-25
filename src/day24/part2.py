import z3
import time

start = time.time_ns()

p = []
v = []

# read input
with open("inputs/day24/input.txt") as f:
    input = f.read().splitlines()
    # split on @
    for line in input:
        parts = line.split("@")
        p_values = [int(x) for x in parts[0].split(",")]
        v_values = [int(x) for x in parts[1].split(",")]
        p.append(p_values)
        v.append(v_values)

# for i in range(len(p)):
#     print(p[i], v[i])

px = z3.Int("px")
py = z3.Int("py")
pz = z3.Int("pz")

vx = z3.Int("vx")
vy = z3.Int("vy")
vz = z3.Int("vz")

solver = z3.Solver()

for i in range(len(p)):
    t = z3.Int("t" + str(i))
    solver.add(t >= 0)
    solver.add(p[i][0] + t * v[i][0] - vx * t == px)
    solver.add(p[i][1] + t * v[i][1] - vy * t == py)
    solver.add(p[i][2] + t * v[i][2] - vz * t == pz)

print(solver.check())
m = solver.model()
print(m[px], m[py], m[pz], "(", m[vx], m[vy], m[vz], ")")

s = m[px].as_long() + m[py].as_long() + m[pz].as_long()
print(f"Result: {s}")

end = time.time_ns()
elapsed_ns = (end - start) / 1000000000
print(f"Time {elapsed_ns} seconds")
