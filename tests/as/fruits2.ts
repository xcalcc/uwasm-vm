let text:string, fLen:i32, i:i32;
let fruits:Array<string> = ["Banana", "Orange", "Apple", "Mango"];
fLen = fruits.length;

text = "<ul>";
for (i = 0; i < fLen; i++) {
    text += "<li>";
    text += fruits[i];
    text += "</li>";
}
text += "</ul>";