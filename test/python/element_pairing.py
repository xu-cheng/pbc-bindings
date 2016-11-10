from helper import *

class TestElementPairing(unittest.TestCase):
    def setUp(self):
        self.pairing = Pairing.init_from_param_str(TYPE_A_PARAM)
        self.e1 = Element(self.pairing, ElementType_G1)
        self.e2 = Element(self.pairing, ElementType_G2)
        self.z = Element(self.pairing, ElementType_Zr)
        self.e1.random()
        self.e2.random()
        self.z.random()

    def test_e(self):
        with self.assertRaises(RuntimeError):
            e(Element(), Element())
        et = e(self.e1, self.e2)
        self.assertEqual(et.type(), ElementType_GT)
        self.assertEqual(et ** self.z, e(self.e1 ** self.z, self.e2))

    def test_e_prod(self):
        with self.assertRaises(ValueError):
            e_prod([], [])
        with self.assertRaises(ValueError):
            e_prod([self.e1, self.e1], [self.e2])
        with self.assertRaises(RuntimeError):
            e_prod([self.e1], [Element()])
        et1 = e_prod([self.e1, self.e1], [self.e2, self.e2])
        et2 = e(self.e1, self.e2)
        self.assertEqual(et1, et2 * et2)

    def test_pairing_fixed_g1(self):
        with self.assertRaises(RuntimeError):
            FixedG1Pairing(Element())
        with self.assertRaises(ValueError):
            FixedG1Pairing(self.z)
        p = FixedG1Pairing(self.e1)
        with self.assertRaises(RuntimeError):
            p.apply(Element())
        self.assertEqual(p.apply(self.e2), e(self.e1, self.e2))

if __name__ == '__main__':
    unittest.main()
