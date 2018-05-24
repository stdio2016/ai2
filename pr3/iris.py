numFeatures = 4
numClasses = 3
def convertClassName(name):
    classes = {
        "Iris-setosa": 0 ,
        "Iris-setosa*": 0, # I don't know why the dataset provided by teacher has this
        "Iris-versicolor": 1,
        "Iris-virginica": 2
    }
    return classes[name]

file = open("iris.txt", "r")
dataset = []
for line in file.readlines():
    line = line.rstrip()
    if line == "": continue # skip empty line
    record = line.split(',')
    out = record[-1]
    record[-1] = convertClassName(out)
    for i in range(numFeatures):
        record[i] = float(record[i])
    dataset.append(record)
