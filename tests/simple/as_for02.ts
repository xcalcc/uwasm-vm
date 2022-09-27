declare function sayHello(): void;

sayHello();

export let q:i32;

export function add(x: i32, y: i32): i32 {
    for (let i:i32 = 0; i < x; i++) {
        for (let j:i32 = 0; j < x; j++) {
            for (let k:i32 = 0; k < x; k++) {
                q = q * y + i * k;
                q = q + j;
            }
        }
    }
    return x / y;
}
