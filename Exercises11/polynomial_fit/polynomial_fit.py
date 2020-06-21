import numpy as np
from matplotlib import pyplot as plt

import tensorflow as tf
from tensorflow import keras

from tensorflow.keras.models import Sequential # sequential model
from tensorflow.keras.layers import Dense, Activation # layers'type and actiavtion function
from tensorflow.keras import optimizers
from tensorflow.keras import backend as K # clear_session(), 
from tensorflow.keras.utils import get_custom_objects

# target parameters of f(x) = a + b*x + c*x**2 + d*x**3
a = 4.
b = -3.
c = -2.
d = 3.
def f(x):
	return a + b*x + c*x**2 + d*x**3

#generating random inputs
#traning and validation data sets

np.random.seed(0)
x_train = np.random.uniform(-1.,1.,10000)
x_valid = np.random.uniform(-1.,1.,1000)
x_valid.sort()

sigma = 0.2
y_train = np.random.normal(f(x_train), sigma)
y_valid = np.random.normal(f(x_valid), sigma)

# plot training and target dataset
print("Plot of the Validation Data Set")

plt.plot(np.arange(-1.1,1.1,0.02), f(np.arange(-1.1,1.1,0.02)), label="Model")
plt.scatter(x_valid, y_valid, color='r', label="Validation Data")
plt.xlabel("x")
plt.ylabel("y")
plt.title("Validation Data")
plt.legend()
plt.grid(True); 
plt.show()

# compose the NN model
#tf.keras.layers.ReLU(max_value=None, negative_slope=0.2, threshold=0)

model = Sequential()
model.add(Dense(8, input_shape=(1,), activation='relu'))
model.add(Dense(6, activation='relu'))
model.add(Dense(6, activation='relu'))
model.add(Dense(6, activation='relu'))
model.add(Dense(6, activation='relu'))
model.add(Dense(1, activation='relu'))



#sgd = optimizers.SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)

model.compile(optimizer=optimizers.SGD(lr=0.01, decay = 1.e-6, momentum=0.0), loss='mse', metrics=['mse'])

model.summary()

# fit the model using training dataset
# over 30 epochs of 32 batch size each
# report training progress against validation data

history = model.fit(x=x_train, y=y_train, batch_size=32, epochs=80, validation_data=(x_valid,y_valid))

# return number of weights and bias
for l in model.layers:
    w = l.get_weights()
    print("layer nodes weights: ", w[0].shape)
    print("layer bias weights: ", w[1].shape)

#model.evaluate(x_valid, y_valid, batch_size=32)
# evaluate model
score = model.evaluate(x_valid, y_valid, batch_size=32, verbose=1)

print()
print('Test loss:', score[0])
print('Test accuracy:', score[1])

# list all data in history
print(history.history.keys())

# summarize history for accuracy
"""
plt.plot(history.history['mse'])      #plt.plot(history.history['acc']) #for generic 'accuracy'
plt.plot(history.history['val_mse'])  #plt.plot(history.history['val_acc'])
plt.ylabel('Model Accuracy')
plt.xlabel('Epoch')
plt.legend(['Train', 'Test'], loc='best')
plt.show()
"""

# look into training history
# Plot training & validation loss values
plt.plot(history.history['loss'])
plt.plot(history.history['val_loss'])
plt.title('Model loss')
plt.ylabel('Loss')
plt.xlabel('Epoch')
plt.legend(['Train', 'Test'], loc='best')
plt.show()

x_predicted = np.arange(-1.3, 1.32, 0.02)
y_predicted = model.predict(x_predicted)
plt.plot(x_predicted, y_predicted,color='r', label="Prediction")
plt.plot(np.arange(-1.30,1.32,0.02), f(np.arange(-1.30,1.32,0.02)), label="Model")
plt.xlabel("x")
plt.ylabel("y")
plt.title("Prediction")
plt.legend()
plt.grid(True)
plt.show()

print()
save_model_path='./Saved/.'
model.save(filepath=save_model_path, include_optimizer=True)