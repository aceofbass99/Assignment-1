using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Healthbar : MonoBehaviour {

    public float maxHealth;
    public float currentHealth;

    private float percentHealth;
    private Material healthBar;

	// Use this for initialization
	void Start () {
        healthBar = GetComponent<Renderer>().material;
        currentHealth = maxHealth;
    }
	
	// Update is called once per frame
	void Update () {
        percentHealth = currentHealth / maxHealth;
        healthBar.SetFloat("_XScale", gameObject.transform.localScale.x);
        healthBar.SetFloat("_Point", percentHealth);
	}
}
