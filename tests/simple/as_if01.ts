declare function sayHello(): void;

sayHello();

export let q:i32;

export function add(x: i32, y: i32): i32 {
    if (x > y)
        q = x + y;
    return x / y;
}
