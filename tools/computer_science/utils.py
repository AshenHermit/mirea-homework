def concatenate_list_to_string(lst):
    return "".join(map(str, lst))

def dec_to_vocab(vocab, dec, length=-1):
    base = len(vocab)
    idxs = []
    n = dec
    while n:
        idxs.append(n%base)
        n = n//base
    idxs += [0]*(max(0, length-len(idxs)))
    idxs = idxs[::-1]
    return list(map(lambda x: vocab[x], idxs))

def word_iterator(vocab, length):
    base = len(vocab)
    for i in range(base**length):
        word = dec_to_vocab(vocab, i, length)
        yield word