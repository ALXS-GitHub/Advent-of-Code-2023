import sympy

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

eq = []
px, py, pz, vx, vy, vz = sympy.symbols("px, py, pz, vx, vy, vz")

for i in range(len(p)):
    eq.append((px - p[i][0]) * (v[i][1] - vy) - (py - p[i][1]) * (v[i][0] - vx)) 
    eq.append((px - p[i][0]) * (v[i][2] - vz) - (pz - p[i][2]) * (v[i][0] - vx))
    
result = sympy.solve(eq)
print(result)
s = result[0][px] + result[0][py] + result[0][pz]
print(f"Result: {s}")    