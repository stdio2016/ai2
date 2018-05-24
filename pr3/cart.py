class CART:
    def __init__(self):
        self.left = None
        self.right = None
        self.feature = 0
        self.threshold = 0.0
        self.classify = -1 # -1 means not leaf, >= 0 means output class
    def leaf(self, output):
        self.classify = output
        return self
    def branch(self, left, right, feature, threshold):
        self.left = left
        self.right = right
        self.feature = feature
        self.threshold = threshold
        return self
    def show(self, indent = 0):
        if self.classify >= 0:
            print ("%sclass %d" % ("\t" * indent, self.classify))
        else:
            print ("%sfeature %d < %f" % ("\t" * indent, self.feature, self.threshold))
            self.left.show(indent+1)
            self.right.show(indent+1)
    def predict(self, record):
        if self.classify >= 0:
            return self.classify
        if record[self.feature] < self.threshold:
            return self.left.predict(record)
        else:
            return self.right.predict(record)

def trainCART(data, features, numClasses, minSize = 1):
    n = len(data)
    minGini = n + 1
    featureToUse = None
    threshold = 0.0
    classCount = [0] * numClasses
    # count number of each class
    for rec in data:
        output = rec[-1]
        classCount[output] += 1
    # if all data belong to one class, return a leaf node
    for c in range(len(classCount)):
        if classCount[c] == n:
            return CART().leaf(c)
    for f in features:
        countSmall = [0] * len(classCount)
        countBig = classCount[0:]
        sortedData = sorted(data, key = lambda rec: rec[f])
        # test each threshold
        for i in range(1, len(data)):
            # count each output class
            output = sortedData[i-1][-1]
            countSmall[output] += 1
            countBig[output] -= 1
            # check feature value
            prev = sortedData[i-1][f]
            current = sortedData[i][f]
            # if previous value is the same, it cannot be a threshold
            if prev == current:
                continue
            # calculate Gini index
            gini = computeGini(countSmall, i) * i + computeGini(countBig, n-i) * (n-i)
            if gini <= minGini:
                minGini = gini
                featureToUse = f
                threshold = (prev + current) / 2.0
    # no way to split
    if featureToUse == None or len(data) < minSize:
        m = 0
        choose = 0
        for c in range(numClasses):
            if classCount[c] > m:
                m = classCount[c]
                choose = c
        return CART().leaf(choose)
    # split data
    left = []
    right = []
    for rec in data:
        if rec[featureToUse] < threshold:
            left.append(rec)
        else:
            right.append(rec)
    leftTree = trainCART(left, features, numClasses, minSize)
    rightTree = trainCART(right, features, numClasses, minSize)
    return CART().branch(leftTree, rightTree, featureToUse, threshold)

def computeGini(count, size):
    return 1.0 - sum(nc * nc for nc in count) / float(size * size)
