from helper import *

class TestElementPower(unittest.TestCase):
    def setUp(self):
        self.pairing = Pairing.init_from_param_str(TYPE_A_PARAM)

    def test_element_power(self):
        base = Element(self.pairing, ElementType_G1)
        e = Element(self.pairing, ElementType_G1)
        z = Element(self.pairing, ElementType_Zr)
        base.random()
        e.random()
        z.random()
        with self.assertRaises(RuntimeError):
            FixedBasePower(Element())
        p = FixedBasePower(base)
        with self.assertRaises(RuntimeError):
            p.apply(Element())
        with self.assertRaises(ValueError):
            p.apply(e)
        self.assertEqual(p.apply(z), base ** z)
        self.assertEqual(p ** z, base ** z)
        self.assertEqual(p.apply(2), base ** 2)
        self.assertEqual(p ** 2, base ** 2)

    def test_element_pow2(self):
        a1 = Element(self.pairing, ElementType_G1)
        a2 = Element(self.pairing, ElementType_G1)
        n1 = Element(self.pairing, ElementType_Zr)
        n2 = Element(self.pairing, ElementType_Zr)
        a1.random()
        a2.random()
        n1.random()
        n2.random()
        with self.assertRaises(RuntimeError):
            pow2(Element(), Element(), Element(), Element())
        with self.assertRaises(ValueError):
            pow2(a1, a1, a2, a2)
        self.assertEqual(pow2(a1, n1, a2, n2), a1.pow(n1) * a2.pow(n2))

    def test_element_pow3(self):
        a1 = Element(self.pairing, ElementType_G1)
        a2 = Element(self.pairing, ElementType_G1)
        a3 = Element(self.pairing, ElementType_G1)
        n1 = Element(self.pairing, ElementType_Zr)
        n2 = Element(self.pairing, ElementType_Zr)
        n3 = Element(self.pairing, ElementType_Zr)
        a1.random()
        a2.random()
        a3.random()
        n1.random()
        n2.random()
        n3.random()
        with self.assertRaises(RuntimeError):
            pow3(Element(), Element(), Element(), Element(), Element(), Element())
        with self.assertRaises(ValueError):
            pow3(a1, a1, a2, a2, a3, a3)
        self.assertEqual(pow3(a1, n1, a2, n2, a3, n3), a1.pow(n1) * a2.pow(n2) * a3.pow(n3))

if __name__ == '__main__':
    unittest.main()
