import argparse


def get_args():
    parser = argparse.ArgumentParser()
    # the environment setting
    parser.add_argument('--device', type=str, default='cuda:8')
    parser.add_argument('--lr', type=float, default=0.0012)
    parser.add_argument('--weight_decay', type=float, default=5e-9)
    parser.add_argument('--batch_size', type=int, default=8)
    parser.add_argument('--num_epochs', type=int, default=200)
    parser.add_argument('--gamma', type=float, default=0.3, help='')
    parser.add_argument('--focal', type=bool, default=True, help='')
    parser.add_argument('--lr_decay', type=bool, default=False, help='if use lr decay')
    parser.add_argument('--sgd_momentum', type=float, default=0.9)
    parser.add_argument('--confusion_matrix', type=bool, default=True, help='if draw the confusion matrix')
    parser.add_argument('--augmentation', type=bool, default=True, help='if use data augmentation')

    args = parser.parse_args()
    return args
