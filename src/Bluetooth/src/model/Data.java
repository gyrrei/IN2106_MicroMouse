package model;

public class Data {
    private int front, right, left;
    private int x, y;
    private int direction;

    public void setFront(int front) {
        this.front = front;
    }

    public void setRight(int right) {
        this.right = right;
    }

    public void setLeft(int left) {
        this.left = left;
    }

    public void setX(int x) {
        this.x = x;
    }

    public void setY(int y) {
        this.y = y;
    }

    public void setDirection(int dir) {
        this.direction = dir;
    }

    public int getFront() {
        return front;
    }

    public int getDirection() {
        return direction;
    }

    public int getRight() {
        return right;
    }

    public int getLeft() {
        return left;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }
}