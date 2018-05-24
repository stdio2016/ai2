numFeatures = 64
numClasses = 10
def convertClassName(name):
    classes = {
        "0": 0,
        "1": 1,
        "2": 2,
        "3": 3,
        "4": 4,
        "5": 5,
        "6": 6,
        "7": 7,
        "8": 8,
        "9": 9
    }
    return classes[name]

file = open("optical-digits.txt", "r")
dataset = []
for line in file.readlines():
    line = line.strip()
    if line == "": continue # skip empty line
    record = line.split(',')
    out = record[-1]
    record[-1] = convertClassName(out)
    for i in range(numFeatures):
        record[i] = float(record[i])
    dataset.append(record)
