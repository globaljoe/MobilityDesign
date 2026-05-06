import furosim

c = furosim.MultirotorClient()
c.confirmConnection()

c.simSetObjectMaterialFromTexture("OrangeBall", "sample_texture.jpg")

