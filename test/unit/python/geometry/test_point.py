"""Unit tests for Point interface"""

# Copyright (C) 2017 Jan Blechta
#
# This file is part of DOLFIN.
#
# DOLFIN is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# DOLFIN is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with DOLFIN. If not, see <http://www.gnu.org/licenses/>.

from __future__ import print_function
import pytest
import numpy as np
from dolfin_utils.test import skip_if_pybind11

from dolfin import Point


def test_point_getitem():
    p = Point(1, 2, 3)
    assert p[0] == 1.0
    assert p[1] == 2.0
    assert p[2] == 3.0
    with pytest.raises(IndexError):
        p[3]
    assert np.all(p[:] == np.array((1.0, 2.0, 3.0)))


def test_point_setitem():
    p = Point()

    p[0] = 6.0
    assert p[0] == 6.0

    p[1] = 16.0
    p[1] += 600.0
    assert np.isclose(p[1], 616.0)

    p[2] = 111.0
    p[2] *= 12.0
    p[2] /= 2
    assert np.isclose(p[2], 666.0)

    with pytest.raises(IndexError):
        p[3] = 6666.0

    p[:] = (0, 0, 0)
    assert np.all(p[:] == 0)

    p[:] = (1, 2, 3)
    assert np.all(p[:] == (1, 2, 3))

    p[:] += np.array((1, 2, 3))
    assert np.all(p[:] == (2, 4, 6))

    p[:] /= 2
    assert np.all(p[:] == (1, 2, 3))

    p[:] *= np.array((2., 2., 2.))
    assert np.all(p[:] == (2, 4, 6))


@skip_if_pybind11
def test_point_array():
    p = Point(1, 2, 3)
    assert np.all(p.array() == (1, 2, 3))

    # Point.array() is a copy, no in-place modification
    p.array()[:] += 1000.0
    assert np.all(p.array() == (1, 2, 3))
