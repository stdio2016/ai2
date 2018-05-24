import cart
import random
import math

# returns tuple (train data, validate data)
def splitTrain(dataset, trainRatio = 0.8):
    n = len(dataset)
    train = []
    validate = list(range(n))
    trainSize = int(len(dataset) * trainRatio)
    for i in range(trainSize):
        r = random.randint(0, n-1-i)
        train.append(validate[r])
        validate[r] = validate[n-1-i]
    validate = validate[0 : n-trainSize]
    train = [dataset[i] for i in train]
    validate = [dataset[i] for i in validate]
    return (train, validate)

def randForest(dataset, numClasses, numFeatures, numTrees):
    forest = []
    featCount = int(math.sqrt(numFeatures))
    for i in range(numTrees):
        attributes = random.sample(range(numFeatures), featCount)
        tree = cart.trainCART(dataset, attributes, numClasses)
        forest.append(tree)
    return forest

def predict(forest, record, numClasses):
    count = [0] * numClasses
    for tree in forest:
        ans = tree.predict(record)
        count[ans] += 1
    m = max(count)
    for i in range(numClasses):
        if count[i] == m:
            return i

def validate(forest, validate, numClasses):
    total = len(validate)
    correct = 0
    for record in validate:
        ans = predict(forest, record, numClasses)
        if ans == record[-1]:
            correct += 1
    print ("accuracy: %f" % (float(correct)/total))
