from helper import *

class TestElementPower(unittest.TestCase):
    def setUp(self):
        self.pairing = Pairing.init_from_param_str(TYPE_A_PARAM)
        self.base = Element(self.pairing, ElementType_G1)
        self.e = Element(self.pairing, ElementType_G1)
        self.z = Element(self.pairing, ElementType_Zr)
        self.base.random()
        self.e.random()
        self.z.random()

    def test_element_power(self):
        with self.assertRaises(RuntimeError):
            FixedBasePower(Element())
        p = FixedBasePower(self.base)
        with self.assertRaises(RuntimeError):
            p.apply(Element())
        with self.assertRaises(ValueError):
            p.apply(self.e)
        self.assertEqual(p.apply(self.z), self.base ** self.z)
        self.assertEqual(p ** self.z, self.base ** self.z)
        self.assertEqual(p.apply(2), self.base ** 2)
        self.assertEqual(p ** 2, self.base ** 2)

if __name__ == '__main__':
    unittest.main()
