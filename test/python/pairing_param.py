from helper import *

class TestPairingParam(unittest.TestCase):
    def test_to_and_init_from_str(self):
        with self.assertRaises(RuntimeError):
            PairingParam.init_from_str("type a")
        param = PairingParam.init_from_str(TYPE_A_PARAM)
        self.assertEqual(param.to_str(), TYPE_A_PARAM)

    def test_gen_type_a(self):
        param = PairingParam.gen_type_a(160, 512)
        self.assertRegex(param.to_str(), "type a")

if __name__ == '__main__':
    unittest.main()
