import numpy as np
import time

## Network architecture
NUM_INPUT = 784  # Number of input neurons
NUM_OUTPUT = 10  # Number of output neurons
NUM_CHECK = 5  # Number of examples on which to check the gradient

## Hyperparameters
NUM_HIDDEN = 100
LEARNING_RATE = 0.2
BATCH_SIZE = 64
NUM_EPOCH = 1000
#DECAY_RATE = 0.6
#DECAY_STEPS = 1000

WHETHER_TEST = False

print("NUM_HIDDEN: ", NUM_HIDDEN)
print("LEARNING_RATE: ", LEARNING_RATE)
print("BATCH_SIZE: ", BATCH_SIZE)
print("NUM_EPOCH: ", NUM_EPOCH)

# Given a vector w containing all the weights and biased vectors, extract
# and return the individual weights and biases W1, b1, W2, b2.
def unpack (w):
    W1 = np.reshape(w[:NUM_INPUT * NUM_HIDDEN],(NUM_INPUT,NUM_HIDDEN))
    w = w[NUM_INPUT * NUM_HIDDEN:]
    b1 = np.reshape(w[:NUM_HIDDEN], NUM_HIDDEN)
    w = w[NUM_HIDDEN:]
    W2 = np.reshape(w[:NUM_HIDDEN*NUM_OUTPUT], (NUM_HIDDEN,NUM_OUTPUT))
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

def softmax(x):
    softmax_x = np.exp(x)
    sum_softmax_x = np.sum(softmax_x)
    softmax_x = softmax_x / sum_softmax_x
    return softmax_x
    
 
def cross_entropy(predicted, ground_truth):
    log_predicted = np.log(predicted)
    loss_matrix = ground_truth * log_predicted
    loss = -np.sum(loss_matrix)
    return loss

def sgn(x):
    result = np.sign(x)
    #print(result)
    result = np.maximum(result,0)
    #print(result)
    return result

def fCE (X, Y, w):
    W1, b1, W2, b2 = unpack(w)
    loss = 0.0
    ## your code here
    global h1_array
    global z2_array
    global z1_array
    global y_predict_array
    h1_array = []
    z1_array = []
    z2_array = []
    y_predict_array = []
    for i in range(len(X)):
        z1 = W1.T.dot(X[i]) + b1
        h1 = np.maximum(z1,0)
        z2 = W2.T.dot(h1) + b2
        y_predict = softmax(z2)
        loss = loss + cross_entropy(y_predict, Y[i])
        h1_array.append(h1)
        z1_array.append(z1)
        z2_array.append(z2)
        y_predict_array.append(y_predict)
        if i == 0:
            if WHETHER_TEST == True:
                print("X:", X.shape)
                print("Y:", Y.shape)
                print("W1:", W1.shape)
                print("W2:", W2.shape)
                print("b1:", b1.shape)
                print("b2:", b2.shape)
                print("h1:", h1.shape)
                print("z1:", z1.shape)
                print("z2:", z2.shape)
                print("y.predict:", y_predict.shape)
    loss = loss / len(X)
    return loss

## 2. Backward Propagation
# Given training images X, associated labels Y, and a vector of combined weights
# and bias terms w, compute and return the gradient of fCE. 
def gradCE (X, Y, w):
    W1, b1, W2, b2 = unpack(w)
    delta_W_1_average = None
    delta_W_2_average = None
    delta_b_1_average = None
    delta_b_2_average = None

    ## your code here
    for i in range(len(X)):
        delta_b_2 = y_predict_array[i] - Y[i]
        y_difference = delta_b_2.reshape(NUM_OUTPUT,1)
        delta_W_2 = y_difference.dot(h1_array[i].reshape(1,NUM_HIDDEN)).T
        a = W2.dot(delta_b_2)
        #print(a.shape)
        #print(np.array(sgn(z1_array[i]).shape))
        b = np.array(a) * np.array(sgn(z1_array[i]))
        delta_W_1 = b.reshape(NUM_HIDDEN,1).dot(X[i].reshape(1,NUM_INPUT))
        delta_b_1 = b
        if i == 0:
            if WHETHER_TEST == True:
                print("delta_W_1:",delta_W_1.shape)
                print("delta_b_1:",delta_b_1.shape)
                print("delta_W_2:",delta_W_2.shape)
                print("delta_b_2:",delta_b_2.shape)
                
            delta_W_1_average = delta_W_1
            delta_W_2_average = delta_W_2
            delta_b_1_average = delta_b_1
            delta_b_2_average = delta_b_2
        else:
            delta_W_1_average = delta_W_1_average + delta_W_1
            delta_W_2_average = delta_W_2_average + delta_W_2
            delta_b_1_average = delta_b_1_average + delta_b_1
            delta_b_2_average = delta_b_2_average + delta_b_2
    delta_W_1 = delta_W_1_average.T / len(X)
    delta_W_2 = delta_W_2_average / len(X)
    delta_b_1 = delta_b_1_average / len(X)
    delta_b_2 = delta_b_2_average / len(X)
    if WHETHER_TEST == True:
        print("delta_W_1:",delta_W_1.shape)
        print("delta_b_1:",delta_b_1.shape)
        print("delta_W_2:",delta_W_2.shape)
        print("delta_b_2:",delta_b_2.shape)
    delta = pack(delta_W_1, delta_b_1, delta_W_2, delta_b_2)
    return delta


def test(testX, testY, w):
    fCE(testX, testY, w)
    total_sum = 0
    for i in range(len(testX)):
        predict = y_predict_array[i]
        ground_truth = testY[i]
        predict_label = np.argmax(predict)
        ground_truth_label = np.argmax(ground_truth)
        if predict_label == ground_truth_label:
            total_sum = total_sum + 1
    success_rate = total_sum / len(testX) * 100
    return success_rate


## 3. Parameter Update
# Given training and testing datasets and an initial set of weights/biases,
# train the NN.
def train(trainX, trainY, testX, testY, w):
    ## your code here
    print("initial test: ", test(testX, testY, w), "%")
    
    for i in range(NUM_EPOCH):
        #更新学习率
        #real_learning_rate = LEARNING_RATE *(DECAY_RATE ** (i / DECAY_STEPS))
        
        
        train_batch_ID = np.arange(len(trainX))
        np.random.shuffle(train_batch_ID)
        train_batch_ID = train_batch_ID[0:BATCH_SIZE]
        #print(train_batch_ID) 
        train_batch_X = trainX[train_batch_ID]
        train_batch_Y = trainY[train_batch_ID]
                
        loss = fCE(train_batch_X, train_batch_Y, w)
        print("current epoch:", i)
        print("current loss:", loss)
        #print("current learning rate:", real_learning_rate)
        delta = gradCE(train_batch_X, train_batch_Y, w)
        w = w - LEARNING_RATE * delta
        if (i + 1) % 100 == 0:
            print("current test: ",test(trainX, trainY,w), "%")
            #最终训练准确率：95.79%
        
    print("final test: ", test(testX, testY, w), "%")
    #最终测试准确率：92.84%
    


if __name__ == "__main__":
    # Load data
    start_time = time.time()
    trainX, trainY = loadData("train")
    testX, testY = loadData("test")

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
