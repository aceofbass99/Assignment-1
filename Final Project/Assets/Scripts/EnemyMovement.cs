using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyMovement : MonoBehaviour {

    public Transform enemy;
    public Transform player;
    public GameObject projectile;
    private float startTime;
    private int x = 0;

	// Use this for initialization
	void Start () {
        startTime = Time.time;
	}
	
	// Update is called once per frame
	void Update () {
        if (Time.time - startTime > 1) {
            //Instantiate(projectile, enemy.position, Quaternion.identity);
            startTime = Time.time;
        }
	}
}
