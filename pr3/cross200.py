numFeatures = 2
numClasses = 2
def convertClassName(name):
    classes = {
        "1": 0,
        "2": 1,
    }
    return classes[name]

file = open("cross200.txt", "r")
dataset = []
for line in file.readlines():
    line = line.strip()
    if line == "": continue # skip empty line
    record = line.split()
    out = record[-1]
    record[-1] = convertClassName(out)
    for i in range(numFeatures):
        record[i] = float(record[i])
    dataset.append(record)
