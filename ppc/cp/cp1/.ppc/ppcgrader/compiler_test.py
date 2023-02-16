import pytest
from .compiler import Compiler


def test_add_definition():
    c = Compiler("cc", [])
    c = c.add_flag("-test-flag")
    # adding one flag to check that we don't disturb things
    assert c.flags == ['-test-flag']

    # simple definition
    cd = c.add_definition("USE_FLOAT")
    assert cd.flags == ['-test-flag', '-DUSE_FLOAT']
    assert c.flags == ['-test-flag']

    # assignment definition
    for value in ["1", 1]:
        cd = c.add_definition("USE_FLOAT", value)
        assert cd.flags == ['-test-flag', '-DUSE_FLOAT=1']
        assert c.flags == ['-test-flag']
