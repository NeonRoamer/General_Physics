# Given values
distance = 3.844e8  # in meters (distance between Earth and Moon)
mass_moon = 7.34767309e22  # in kilograms (mass of the Moon)
mass_earth = 5.972e24  # in kilograms (mass of the Earth)

# Calculate the barycenter distance from Earth's center
barycenter_distance = (distance * mass_moon) / (mass_moon + mass_earth)
print(barycenter_distance)
