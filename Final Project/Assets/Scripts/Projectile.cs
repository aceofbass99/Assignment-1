using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Projectile : MonoBehaviour
{
    static AudioSource source;
    public AudioClip shoot;
    private Vector2 mousePosition;
    private Vector2 direction;
    public float speed;
    public static AudioClip shootSound;
    // Use this for initialization
    void Start()
    {
        shootSound = Resources.Load<AudioClip>("playershoot");
        source = GetComponent<AudioSource>();
        source.PlayOneShot(shootSound);
        mousePosition = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        direction = mousePosition - (Vector2)transform.position;
        direction.Normalize();
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(direction * speed * Time.deltaTime);
    }

    void OnBecameInvisible()
    {
        Destroy(this.gameObject);
    }
}
