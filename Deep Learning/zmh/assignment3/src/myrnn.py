import torch
import torch.nn.functional as F


def rnn_cell_forward(x, h, params):

    Whx = params["Whx"]
    Whh = params["Whh"]
    Wyh = params["Wyh"]
    bh = params["bh"]
    by = params["by"]

    h_next = F.tanh(torch.matmul(Whx, x) + torch.matmul(Whh, h) + bh)
    y = F.softmax(torch.matmul(Wyh, h_next) + by, 1)
    cache = (h_next, h, x, params)

    return h_next, y, cache


def rnn_forward(x, h0, params):

    caches = []
    n_x, m, t_x = x.shape
    n_y, n_h = params["Wyh"].shape

    h = torch.zeros((n_h, m, t_x))
    y = torch.zeros((n_y, m, t_x))
    h_next = h0

    for t in range(t_x):
        h_next, yt, cache = rnn_cell_forward(x[:, :, t], h_next, params)
        h[:, :, t] = h_next
        y[:, :, t] = yt
        caches.append(cache)

    # store values needed for backward propagation in cache
    caches = (caches, x)

    return h, y, caches


def rnn_cell_backward(dh, cache):

    (h, h, xt, params) = cache

    Whx = params["Whx"]
    Whh = params["Whh"]

    dtanh = (1 - h ** 2) * dh

    dxt = torch.matmul(Whx.T, dtanh)
    dWhx = torch.matmul(dtanh, xt.T)

    dh = torch.matmul(Whh.T, dtanh)
    dWhh = torch.matmul(dtanh, h.T)

    dbh = dtanh.sum(axis=1, keepdims=True)

    gradients = {"dxt": dxt, "dh": dh, "dWhx": dWhx, "dWhh": dWhh, "dbh": dbh}

    return gradients


def rnn_backward(dh, caches):

    (caches, x) = caches
    (h1, h0, x1, params) = caches[0]
    n_h, m, T_x = dh.shape
    n_x, m = x1.shape
    dx = torch.zeros((n_x, m, T_x))
    dWhx = torch.zeros((n_h, n_x))
    dWhh = torch.zeros((n_h, n_h))
    dbh = torch.zeros((n_h, 1))
    dh_prevt = torch.zeros((n_h, m))

    for t in reversed(range(T_x)):
        gradients = rnn_cell_backward(dh[:, :, t] + dh_prevt, caches[t])
        dxt, dh_prevt, dWhxt, dWhht, dbht = gradients["dxt"], gradients["dh"], gradients["dWhx"], gradients[
            "dWhh"], gradients["dbh"]
        dx[:, :, t] = dxt
        dWhx += dWhxt
        dWhh += dWhht
        dbh += dbht

    dh0 = dh_prevt
    gradients = {"dx": dx, "dh0": dh0, "dWhx": dWhx, "dWhh": dWhh, "dbh": dbh}

    return gradients


if __name__ == '__main__':
    x = torch.randn(3, 10, 100)
    h0 = torch.randn(5, 10)
    Whx = torch.randn(5, 3)
    Whh = torch.randn(5, 5)
    Wyh = torch.randn(2, 5)
    bh = torch.randn(5, 1)
    by = torch.randn(2, 1)
    parameters = {"Whx": Whx, "Whh": Whh, "Wyh": Wyh, "bh": bh, "by": by}
    h, y, caches = rnn_forward(x, h0, parameters)
    print("hidden =", h)
    print("prediction =", y)
    dh = torch.randn(5, 10, 4)
    gradients = rnn_backward(dh, caches)

    print("gradients[\"dx\"][1][2] =", gradients["dx"][1][2])
    print("gradients[\"dx\"].shape =", gradients["dx"].shape)
    print("gradients[\"dh0\"][2][3] =", gradients["dh0"][2][3])
    print("gradients[\"dh0\"].shape =", gradients["dh0"].shape)
    print("gradients[\"dWhx\"][3][1] =", gradients["dWhx"][3][1])
    print("gradients[\"dWhx\"].shape =", gradients["dWhx"].shape)
    print("gradients[\"dWhh\"][1][2] =", gradients["dWhh"][1][2])
    print("gradients[\"dWhh\"].shape =", gradients["dWhh"].shape)
    print("gradients[\"dbh\"][4] =", gradients["dbh"][4])
    print("gradients[\"dbh\"].shape =", gradients["dbh"].shape)
