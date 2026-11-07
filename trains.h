#ifndef TRAINS_H
#define TRAINS_H

namespace trains_details {

int hidden_dummy_fn() {
    return 123;
}

} // namespace

int dummy_fn() {
    return trains_details::hidden_dummy_fn();
}

#endif
