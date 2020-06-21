import numpy as np
import matplotlib.pyplot as plt

import tensorflow as tf
from tensorflow import keras

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Activation
from tensorflow.keras import backend as K
from tensorflow.keras.utils import get_custom_objects

# target parameters of f(x) = m*x + b
m = 2 # slope
q = 1 # intersect

def f(x):
	return m*x + q

print("Neural Network for Linear Fit")
print("Parameters of the linear models:")
print("Slope:\t\t2\nIntersect:\t1")

# generate training inputs
np.random.seed(0)
x_train = np.random.uniform(-1, 1, 1000)
x_valid = np.random.uniform(-1, 1, 100)
x_valid.sort()
y_target = m * x_valid + q # ideal (target) linear function

sigma = 0.6 # noise standard deviation, for the moment it is absent
y_train = np.random.normal(m * x_train + q, sigma) # actual measures from which we want to guess regression parameters
y_valid = np.random.normal(m * x_valid + q, sigma)

# plot training and target dataset
print("Plot of the Training and Validation Data Sets")

plt.plot(x_valid, y_target)
plt.scatter(x_valid, y_valid, color='r')
plt.xlabel("x")
plt.ylabel("y")
plt.title("Validation Data")
plt.grid(True); plt.show()

# compose the NN model

model = keras.Sequential()
model.add(Dense(1, input_shape=(1,)))

# compile the model choosing optimizer, loss and metrics objects
model.compile(optimizer='sgd', loss='mse', metrics=['mse'])

# get a summary of our composed model
model.summary()

# fit the model using training dataset
# over 30 epochs of 32 batch size each
# report training progress against validation data
history = model.fit(x=x_train, y=y_train, 
          batch_size=32, epochs=50,
          shuffle=True, # a good idea is to shuffle input before at each epoch
          validation_data=(x_valid, y_valid))

# return weights
print(model.get_weights())

# evaluate model
score = model.evaluate(x_valid, y_valid, batch_size=32, verbose=1)

# print performance
print()
print('Test loss:', score[0])
print('Test accuracy:', score[1])

# look into training history
# Plot training & validation loss values
plt.plot(history.history['loss'])
plt.plot(history.history['val_loss'])
plt.title('Model loss')
plt.ylabel('Loss')
plt.xlabel('Epoch')
plt.legend(['Train', 'Test'], loc='best')
plt.show()

x_predicted = np.random.uniform(-1, 1, 100)
y_predicted = model.predict(x_predicted)
plt.scatter(x_predicted, y_predicted,color='r', label='Predicted')
plt.plot(x_valid, y_target, label='Model')
plt.grid(True)
plt.xlabel("x")
plt.ylabel("y")
plt.title("Prediction")
plt.legend()
plt.show()

K.clear_session()