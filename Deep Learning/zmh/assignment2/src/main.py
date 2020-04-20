import torch
import torch.nn as nn
import torch.optim as optim
import data
import models
from tensorboardX import SummaryWriter
from datetime import datetime
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sn
import pandas as pd
from arguments import get_args


## Note that: here we provide a basic solution for training and validation.
## You can directly change it if you find something wrong or not good enough.

def train_model(args, model, train_loader, valid_loader, criterion, optimizer, num_epochs=20, writer=None,
                scheduler=None):
    def train(model, train_loader, optimizer, criterion):
        model.train(True)
        total_loss = 0.0
        total_correct = 0

        for inputs, labels in train_loader:
            inputs = inputs.to(device)
            labels = labels.to(device)
            optimizer.zero_grad()
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            if args.focal:
                pt = torch.exp(-loss)
                loss = (1.0 * (1 - pt) ** 1.0 * loss).mean()  # mean over the batch
            _, predictions = torch.max(outputs, 1)
            loss.backward()
            optimizer.step()

            total_loss += loss.item() * inputs.size(0)
            total_correct += torch.sum(predictions == labels.data)

        epoch_loss = total_loss / len(train_loader.dataset)
        epoch_acc = total_correct.double() / len(train_loader.dataset)
        return epoch_loss, epoch_acc.item()

    def valid(model, valid_loader, criterion):
        model.train(False)
        total_loss = 0.0
        total_correct = 0
        for inputs, labels in valid_loader:
            inputs = inputs.to(device)
            labels = labels.to(device)
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            if args.focal:
                pt = torch.exp(-loss)
                loss = (0.5 * (1 - pt) ** 2.0 * loss).mean()  # mean over the batch
            _, predictions = torch.max(outputs, 1)
            total_loss += loss.item() * inputs.size(0)
            total_correct += torch.sum(predictions == labels.data)
        epoch_loss = total_loss / len(valid_loader.dataset)
        epoch_acc = total_correct.double() / len(valid_loader.dataset)
        return epoch_loss, epoch_acc.item()

    def draw_confusion_matrix(model, valid_loader, num_classes):
        conf_matrix = torch.zeros(num_classes, num_classes)

        model.train(False)
        for inputs, labels in valid_loader:
            inputs = inputs.to(device)
            labels = labels.to(device)
            outputs = model(inputs)
            conf_matrix = confusion_matrix(outputs, labels, conf_matrix)
            df_cm = pd.DataFrame(conf_matrix.numpy(),
                                 index=[i for i in range(num_classes)],
                                 columns=[i for i in range(num_classes)])
            plt.figure(figsize=(10, 7))
            sn.heatmap(df_cm, annot=True, cmap="BuPu")
            plt.savefig('confusion_matrix.png')

    best_acc = 0.0
    for epoch in range(num_epochs):
        if scheduler:
            scheduler.step()
        print('epoch:{:d}/{:d}'.format(epoch, num_epochs))
        print('*' * 100)
        train_loss, train_acc = train(model, train_loader, optimizer, criterion)
        writer.add_scalar('train_loss', train_loss, epoch)
        writer.add_scalar('train_acc', train_acc, epoch)

        print("training: {:.4f}, {:.4f}".format(train_loss, train_acc))
        valid_loss, valid_acc = valid(model, valid_loader, criterion)
        writer.add_scalar('valid_loss', valid_loss, epoch)
        writer.add_scalar('valid_acc', valid_acc, epoch)

        print("validation: {:.4f}, {:.4f}".format(valid_loss, valid_acc))
        if valid_acc > best_acc:
            best_acc = valid_acc
            best_model = model
            torch.save(best_model.state_dict(), 'test_C.pt')
    if args.confusion_matrix:
        draw_confusion_matrix(model, valid_loader, num_classes)


def confusion_matrix(preds, labels, conf_matrix):
    preds = torch.argmax(preds, 1)
    for p, t in zip(preds, labels):
        conf_matrix[p, t] += 1
    return conf_matrix


if __name__ == '__main__':
    args = get_args()

    ## about model
    num_classes = 20

    ## about data
    data_dir = "../data/"
    inupt_size = 224
    batch_size = args.batch_size

    ## about training
    num_epochs = args.num_epochs
    lr = args.lr

    ## model initialization
    model = models.model_C(num_classes=num_classes)
    device = torch.device(args.device if torch.cuda.is_available() else "cpu")
    model = model.to(device)
    writer = SummaryWriter('./logdir/C_' + datetime.now().strftime('%b%d_%H-%M-%S'))
    ## data preparation
    train_loader, valid_loader = data.load_data(data_dir=data_dir, input_size=inupt_size, batch_size=batch_size,
                                                augmentation=args.augmentation)

    ## optimizer
    optimizer = optim.Adam(model.parameters(), lr=lr, weight_decay=args.weight_decay)
    # optimizer = optim.SGD(model.parameters(), lr=lr, momentum=args.sgd_momentum)

    ## scheduler
    if args.lr_decay:
        scheduler = torch.optim.lr_scheduler.StepLR(optimizer, step_size=50, gamma=args.gamma)
    else:
        scheduler = None

    ## loss function
    if args.focal:
        criterion = nn.CrossEntropyLoss(reduce=False)
    else:
        criterion = nn.CrossEntropyLoss()
    train_model(args, model, train_loader, valid_loader, criterion, optimizer, num_epochs=num_epochs, writer=writer,
                scheduler=scheduler)
