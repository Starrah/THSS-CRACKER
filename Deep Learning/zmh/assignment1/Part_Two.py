from sklearn.utils import shuffle
import numpy as np
import time


## Network architecture
NUM_INPUT = 784  # Number of input neurons
NUM_OUTPUT = 10  # Number of output neurons
NUM_CHECK = 5  # Number of examples on which to check the gradient

## Hyperparameters
NUM_HIDDEN = 50
LEARNING_RATE = 0.01 ## weight decay works better!
BATCH_SIZE = 64
NUM_EPOCH = 40

print("NUM_HIDDEN: ", NUM_HIDDEN)
print("LEARNING_RATE: ", LEARNING_RATE)
print("BATCH_SIZE: ", BATCH_SIZE)
print("NUM_EPOCH: ", NUM_EPOCH)

# Given a vector w containing all the weights and biased vectors, extract
# and return the individual weights and biases W1, b1, W2, b2.
def unpack (w):
    W1 = np.reshape(w[:NUM_INPUT * NUM_HIDDEN],(NUM_HIDDEN, NUM_INPUT))
    w = w[NUM_INPUT * NUM_HIDDEN:]
    b1 = np.reshape(w[:NUM_HIDDEN], NUM_HIDDEN)
    w = w[NUM_HIDDEN:]
    W2 = np.reshape(w[:NUM_HIDDEN*NUM_OUTPUT], (NUM_OUTPUT, NUM_HIDDEN))
    w = w[NUM_HIDDEN*NUM_OUTPUT:]
    b2 = np.reshape(w,NUM_OUTPUT)
    return W1, b1, W2, b2

# Given individual weights and biases W1, b1, W2, b2, concatenate them and
# return a vector w containing all of them.
def pack (W1, b1, W2, b2):
    W1_ = np.reshape(W1,NUM_INPUT*NUM_HIDDEN)
    # print(W1_.shape)
    W2_ = np.reshape(W2,NUM_HIDDEN*NUM_OUTPUT)
    # print(W2_.shape)
    w = np.concatenate((W1_,b1, W2_, b2))
    # print(w.shape)
    return w

# Load the images and labels from a specified dataset (train or test).
def loadData (which):
    images = np.load("./data/mnist_{}_images.npy".format(which))
    labels = np.load("./data/mnist_{}_labels.npy".format(which))
    return images, labels

## 1. Forward Propagation
# Given training images X, associated labels Y, and a vector of combined weights
# and bias terms w, compute and return the cross-entropy (CE) loss.
def softmax(z):
    z_exp = np.exp(z)
    z_exp_sum = np.sum(z_exp, 0)
    return z_exp / z_exp_sum

def relu(z):
    return np.array([([[0, zij][zij > 0] for zij in zi]) for zi in z])

def cross_entropy(p, q):
    x = np.sum(np.multiply(np.log(p.T), q), 1)
    loss = np.mean(x)
    return -loss

def fCE (X, Y, w):
    # print(X.shape)
    W1, b1, W2, b2 = unpack(w)
    loss = 0.0
    ## your code here

    ## forward 1
    z1 = (np.dot(W1, X).T + b1).T
    a1 = relu(z1)

    ## forward 2
    z2 = (np.dot(W2, a1).T + b2).T
    a2 = softmax(z2)

    loss = cross_entropy(a2, Y)

    y_gt = [np.argmax(it) for it in Y]
    y_pred = [np.argmax(it) for it in a2.T]

    correct = [i == j for (i, j) in zip(y_pred, y_gt)]
    ac = np.mean(correct)
    
    return loss, z1, a1, z2, a2, ac

## 2. Backward Propagation
# Given training images X, associated labels Y, and a vector of combined weights
# and bias terms w, compute and return the gradient of fCE. 

def gradrelu(x):
    g = np.array([([[0, 1][j > 0] for j in i]) for i in x])
    return g

def gradCE (X, Y, w, z1, a1, z2, a2):
    W1, b1, W2, b2 = unpack(w)
    ## your code here

    ## grad for residual 2
    r2 = a2.T - Y
    d_W2_all = np.array([np.dot(r2_i[:, None], a1_i[None, :]) for (r2_i, a1_i) in zip(r2, a1.T)])
    d_W2 = np.mean(d_W2_all, axis=0)
    d_b2 = np.mean(r2, axis=0)
    ## grad for residual 1
    r1 = np.multiply(gradrelu(z1), np.dot(W2.T, r2.T))
    d_W1_all = np.array([np.dot(r1_i[:, None], X_i[None, :]) for (r1_i, X_i) in zip(r1.T, X.T)])
    d_W1 = np.mean(d_W1_all, axis=0)
    d_b1 = np.mean(r1, axis=1)
    
    delta = pack(d_W1, d_b1, d_W2, d_b2)
    return delta

## 3. Parameter Update
# Given training and testing datasets and an initial set of weights/biases,
# train the NN.
def train(trainX, trainY, testX, testY, w):
    ## your code here
    for ep in range(NUM_EPOCH):
        train_loss = 0
        ac = 0
        trainX, trainY = shuffle(trainX, trainY)
        splits_x = np.array_split(trainX, trainX.shape[0] / BATCH_SIZE)
        splits_y = np.array_split(trainY, trainY.shape[0] / BATCH_SIZE)
        for i, j in zip(splits_x, splits_y):
            i = i.T
            train_loss, z1, a1, z2, a2, ac = fCE(i, j, w)
            grad = gradCE(i, j, w, z1, a1, z2, a2)
            w -= LEARNING_RATE * grad

        print("{} Epochs: \n".format(ep + 1))

        print("train loss: {}, train accuracy: {}\n".format(train_loss, ac))

        test_loss, _, _, _, _, ac = fCE(testX.T, testY, w)
        print("test loss: {}, test accuracy: {}\n".format(test_loss, ac))
    

if __name__ == "__main__":
    # Load data
    start_time = time.time()
    trainX, trainY = loadData("train")
    testX, testY = loadData("test")
    
    print(trainY.shape)

    print("len(trainX): ", len(trainX))
    print("len(testX): ", len(testX))

    # Initialize weights randomly
    W1 = 2*(np.random.random(size=(NUM_INPUT, NUM_HIDDEN))/NUM_INPUT**0.5) - 1./NUM_INPUT**0.5
    b1 = 0.01 * np.ones(NUM_HIDDEN)
    W2 = 2*(np.random.random(size=(NUM_HIDDEN, NUM_OUTPUT))/NUM_HIDDEN**0.5) - 1./NUM_HIDDEN**0.5
    b2 = 0.01 * np.ones(NUM_OUTPUT)

    w = pack(W1, b1, W2, b2)
    print("Shape of w:",w.shape)

    # # Train the network and report the accuracy on the training and test set.
    train(trainX, trainY, testX, testY, w)

    ## report:
    ## 40 epochs
    ## train accuracy: 0.953125
    ## train loss: 0.233849146612
    ## test 0.953125: 0.9116
    ## test loss: 0.315527667202