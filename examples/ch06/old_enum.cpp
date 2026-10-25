enum Weapon { Pistol, Shotgun };
enum Pickup { Medkit, Shotgun };    // Shotgun уже занято!

int main() {
    int damage = Pistol * 10;        // старый enum молча стал числом
    return damage;
}
