import numpy as np
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import tensorflow as tf
from tensorflow import keras

from tensorflow.keras.models import Sequential # sequential model
from tensorflow.keras.layers import Dense, Activation # layers'type and actiavtion function
from tensorflow.keras import optimizers
from tensorflow.keras import backend as K # clear_session(), 
from tensorflow.keras.utils import get_custom_objects

def f(x,y):
	return np.sin(x*x+y*y)

#generating random inputs
#traning and validation data sets

np.random.seed(0)
n_train = 12000
n_valid = 1200

x_train = np.zeros((n_train, 2))
x_valid = np.zeros((n_valid, 2))
y_train = np.zeros(n_train)
y_valid = np.zeros(n_valid)

sigma = 0.2

for i in range(n_train):
	x_train[i,0] = np.random.uniform(-1.5,1.5)
	x_train[i,1] = np.random.uniform(-1.5,1.5)
	y_train[i] = np.random.normal(f(x_train[i,0],x_train[i,1]),sigma)

for i in range(n_valid):
	x_valid[i,0] = np.random.uniform(-1.5,1.5)
	x_valid[i,1] = np.random.uniform(-1.5,1.5)
	y_valid[i] = np.random.normal(f(x_valid[i,0],x_valid[i,1]),sigma)

# plot training and target dataset
print("Plot of the Validation Data Sets")

x_plot = np.arange(-1.5,1.53, 0.03)
y_plot = np.arange(-1.5,1.53, 0.03)
X_plot, Y_plot = np.meshgrid(x_plot,y_plot)

fig = plt.figure(figsize=(8,8))
ax = fig.add_subplot(111, projection='3d')

ax.plot_wireframe(X_plot, Y_plot, f(X_plot,Y_plot), rstride=2, cstride=2, label='Model')
ax.scatter(np.transpose(x_valid)[0], np.transpose(x_valid)[1], y_valid, color='red', marker='.', label='Validation Data')
ax.set_xlabel('x', fontsize=20)
ax.set_ylabel('y', fontsize=20)
ax.set_zlabel('z', fontsize=20)
ax.text2D(0.05, 0.95, "Validation Data", transform=ax.transAxes, fontsize=20)
ax.view_init(20, 30)
ax.legend()
plt.show()

# compose the NN model

model = Sequential()
model.add(Dense(40, input_shape=(2,), activation='relu'))
model.add(Dense(35, activation='relu'))
model.add(Dense(30, activation='relu'))
model.add(Dense(1))

#sgd = optimizers.SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)

model.compile(optimizer=optimizers.SGD(lr=0.01, decay=1.e-6, momentum=0.5), loss='mse', metrics=['mse'])

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

# look into training history
# Plot training & validation loss values
plt.plot(history.history['loss'])
plt.plot(history.history['val_loss'])
plt.title('Model loss')
plt.ylabel('Loss')
plt.xlabel('Epoch')
plt.legend(['Train', 'Test'], loc='best')
plt.show()

print()
print('Test loss:', score[0])
print('Test accuracy:', score[1])

x_predicted = np.zeros((n_valid,2))
y_target = np.zeros(n_valid)

for i in range(n_valid):
	x_predicted[i,0] = np.random.uniform(-1.5,1.5)
	x_predicted[i,1] = np.random.uniform(-1.5,1.5)
	y_target[i] = f(x_predicted[i,0],x_predicted[i,1])
	
y_predicted = np.transpose(model.predict(x_predicted))
y_predicted -= y_target

fig = plt.figure(figsize=(11,8))
ax = fig.add_subplot(111, projection='3d')

ax.scatter(np.transpose(x_predicted)[0], np.transpose(x_predicted)[1], y_predicted, color='red', marker='.', label='Validation Data')
ax.set_xlabel('x', fontsize=20)
ax.set_ylabel('y', fontsize=20)
ax.set_zlabel('z', fontsize=20)
ax.text2D(0.05, 0.95, "Prediction Error", transform=ax.transAxes, fontsize=20)
ax.view_init(20, 30)
plt.show()

# list all data in history
print(history.history.keys())

print()
save_model_path='./Saved/.'
model.save(filepath=save_model_path, include_optimizer=True)