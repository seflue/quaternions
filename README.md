# Quaternions

This is just a small finger exercise to get familiar with the algebraic rules of quaternions.

## Supported and planned operations

- [x] addition (overload of binary `+` operator)
- [x] subtraction (overload of binary `-` operator)
- [ ] multiplications
  - [x] common multiplication or Grassmann product (overload of binary `*` operator) 
  - [x] cross product or Grassmann uneven product (`q1.cross(q2)`)
  - [x] dot product or Euclidean even product (`q1.dot(q2)`)
  - [ ] _Grassmann even product_
  - [ ] _Euclidean uneven product_
- [x] conversion from and to vector
- [x] conversion from and to rotation
- [x] norm (`.norm()`)
- [x] normalization
- [x] vector extraction
- [x] scalar extraction
- [x] inverse
- [x] negation
- [x] rotation
- [ ] _direction_
- [ ] _Euler angle representation_

# Further improvements

- [ ] implementation of double quaternions
- [ ] benchmarks
